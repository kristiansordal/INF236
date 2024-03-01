#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *permuted = (ull *)malloc(n * sizeof(ull));

    if (a == NULL) {
        fprintf(stderr, "Failed to allocate memory for main array\n");
        exit(EXIT_FAILURE);
    }
    if (permuted == NULL) {
        fprintf(stderr, "Failed to allocate memory for permutation array\n");
        exit(EXIT_FAILURE);
    }

    const int buckets = 1 << b;
    int p = 0;

#pragma omp parallel
    {
#pragma omp critical
        { p = omp_get_num_threads(); }
    }

    int *histogram = (int *)malloc(p * buckets * sizeof(int));

    // Generate random 64 bit integers
    init_rand(a, n);
    const double start = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
#pragma omp for
            for (int i = 0; i < p * buckets; i++)
                histogram[i] = 0;
#pragma omp for
            for (int i = 0; i < n; i++) {
                histogram[tid * buckets + ((a[i] >> shift) & (buckets - 1))]++;
            }

#pragma omp barrier
#pragma omp master
            {
                int s = 0;
                for (int i = 0; i < buckets; i++) {
                    for (int j = 0; j < p; j++) {
                        const int idx = j * buckets + i; // Correct index in flattened array
                        const int t = s + histogram[idx];
                        histogram[idx] = s;
                        s = t;
                    }
                }
            }

#pragma omp barrier
#pragma omp for
            for (int i = 0; i < n; i++) {
                ull val = a[i];
                int t = (val >> shift) & (buckets - 1);
                permuted[histogram[tid * buckets + t]++] = val;
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
    free(permuted);
    // for (int i = 0; i < p; i++)
    //     free(histogram[i]);
    free(histogram);
    // free(begins);
    // free(ends);
    return end - start;
}
