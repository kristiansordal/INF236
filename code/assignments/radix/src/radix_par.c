#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void compute_ranges(ull *begin, ull *end, ull n, ull p) {
    begin[0] = 0;
    for (ull i = 0; i < p - 1; i++) {
        end[i] = (i + 1) * n / p;
        begin[i + 1] = end[i];
    }

    end[p - 1] = n;
}

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *permuted = (ull *)malloc(n * sizeof(ull));

    if (a == NULL || permuted == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    } else {
        // printf("Allocated memory for %llu elements\n", n);
    }

    const ull buckets = 1 << b;
    ull p;

#pragma omp parallel
    {
#pragma omp master
        { p = omp_get_num_threads(); }
    }

    ull histogram[p][buckets]; // Sub-bucket-size table
    ull bs[buckets];           // Bucket size table
    ull out_buf[p][buckets];

    ull begins[p], ends[p];
    compute_ranges(begins, ends, n, p);

    // Generate random 64 bit ullegers
    init_rand(a, n);
    const double start = omp_get_wtime();

    for (ull shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const ull tid = omp_get_thread_num();
            ull_init(histogram[tid], buckets);
            ull_init(bs, buckets);

            for (ull i = begins[tid]; i < ends[tid]; i++)
                histogram[tid][(a[i] >> shift) & (buckets - 1)]++;

#pragma omp barrier

#pragma omp master
            {
                ull s = 0;
                for (ull i = 0; i < buckets; i++) {
                    for (ull j = 0; j < p; j++) {
                        const ull t = s + histogram[j][i];
                        histogram[j][i] = s;
                        s = t;
                    }
                    bs[i] = s;
                }
            }

#pragma omp barrier

            ull *histo_tid = histogram[tid];
            for (ull i = begins[tid]; i < ends[tid]; i++) {
                ull val = a[i];                         // get value
                ull t = (val >> shift) & (buckets - 1); // get bucket
                permuted[histo_tid[t]++] = val;
            }
        }

        ull *swap = a;
        a = permuted;
        permuted = swap;
    }

    const double end = omp_get_wtime();

    if (n <= 20) {
        for (ull i = 0; i < n; i++) {
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
