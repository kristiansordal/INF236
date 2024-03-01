#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parallel radix sort
double radix_sort_par(int n, int b) {
    // After modification
    ull *a = NULL;
    ull *permuted = NULL;
    size_t alignment = 32; // Cache line size for alignment

    if (posix_memalign((void **)&a, alignment, n * sizeof(ull)) != 0) {
        fprintf(stderr, "Failed to allocate aligned memory for main array\n");
        exit(EXIT_FAILURE);
    }

    if (posix_memalign((void **)&permuted, alignment, n * sizeof(ull)) != 0) {
        fprintf(stderr, "Failed to allocate aligned memory for permutation array\n");
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
#pragma omp for schedule(static, buckets)
            for (int i = 0; i < p * buckets; i++)
                histogram[i] = 0;

#pragma omp for nowait schedule(static, n / p)
            for (int i = 0; i < n; i++)
                histogram[tid * buckets + ((a[i] >> shift) & (buckets - 1))]++;

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
#pragma omp for schedule(static, n / p)
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
    free(histogram);
    return end - start;
}
