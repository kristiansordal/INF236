#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void compute_ranges(int *begin, int *end, int n, int p) {
    begin[0] = 0;
    for (int i = 0; i < p - 1; i++) {
        end[i] = (i + 1) * n / p;
        begin[i + 1] = end[i];
    }

    end[p - 1] = n;
}

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *permuted = (ull *)malloc(n * sizeof(ull));
    const int buckets = 1 << b;
    int p;

#pragma omp parallel
    {
#pragma omp critical
        { p = omp_get_num_threads(); }
    }

    int histogram[p][buckets]; // Sub-bucket-size table
    int bs[buckets];           // Bucket size table
    int pfs[buckets];          // Prefix sum table
    ull out_buf[p][buckets];

    int begins[p], ends[p];
    compute_ranges(begins, ends, n, p);

    // Generate random 64 bit integers
    init_rand(a, n);
    const double start = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            int_init(histogram[tid], buckets);
            int_init(bs, buckets);
            int_init(pfs, buckets);

            for (int i = begins[tid]; i < ends[tid]; i++)
                histogram[tid][(a[i] >> shift) & (buckets - 1)]++;
        }

        int s = 0;
        for (int i = 0; i < buckets; i++) {
            for (int j = 0; j < p; j++) {
                const int t = s + histogram[j][i];
                histogram[j][i] = s;
                s = t;
            }
            bs[i] = s;
        }

#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            int *histo_tid = histogram[tid];

            for (int i = begins[tid]; i < ends[tid]; i++) {
                ull val = a[i];                         // get value
                int t = (val >> shift) & (buckets - 1); // get bucket
                permuted[histo_tid[t]++] = val;
            }
        }

        ull *swap = a;
        a = permuted;
        permuted = swap;
    }

    const double end = omp_get_wtime();

    if (n <= 20) {
        for (int i = 0; i < n; i++) {
            printf("%llu\n", a[i]);
        }
    }

    if (is_sorted(a, n) == 1)
        printf("PARALLEL: Success!\n");
    else
        printf("PARALLEL: Failed!\n");

    free(a);
    return end - start;
}
