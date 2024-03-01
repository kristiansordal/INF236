#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (a == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Allocated a\n");
    }
    ull *permuted = (ull *)malloc(n * sizeof(ull));
    if (permuted == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Allocated permuted\n");
    }

    const int buckets = 1 << b;
    int p;

#pragma omp parallel
    {
#pragma omp master
        { p = omp_get_num_threads(); }
    }

    int histogram[p][buckets]; // Sub-bucket-size table
    int bs[buckets];           // Bucket size table

    int begins[p], ends[p];
    compute_ranges(begins, ends, n, p);

    // Generate random 64 bit integers
    init_rand(a, n);
    const double start = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            // memset(histogram[tid], 0, buckets * sizeof(int));
            // memset(bs, 0, buckets * sizeof(int));
            int_init(histogram[tid], buckets);
            int_init(bs, buckets);

            for (int i = begins[tid]; i < ends[tid]; i++)
                histogram[tid][(a[i] >> shift) & (buckets - 1)]++;

#pragma omp barrier

#pragma omp master
            {
                int s = 0;
                for (int i = 0; i < buckets; i++) {
                    for (int j = 0; j < p; j++) {
                        const int t = s + histogram[j][i];
                        histogram[j][i] = s;
                        s = t;
                    }
                    bs[i] = s;
                }
            }

#pragma omp barrier

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

    // printf("HISTOGRAM: %f\n", histo_t);
    // printf("SCAN: %f\n", scan_t);
    // printf("PERMUTE: %f\n", permute_t);
    free(a);
    free(permuted);
    return end - start;
}
