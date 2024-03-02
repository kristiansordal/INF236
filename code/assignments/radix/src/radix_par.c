#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_ranges(size_t *begins, size_t *ends, int n, int p) {
    begins[0] = 0;
    for (int i = 0; i < p - 1; i++) {
        ends[i] = (i + 1) * (n / p);
        begins[i + 1] = ends[i];
    }
    ends[p - 1] = n;
}

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

    size_t **histogram = (size_t **)malloc(p * sizeof(size_t *));
    for (int i = 0; i < p; i++)
        histogram[i] = (size_t *)malloc(buckets * sizeof(size_t));

    size_t ends[p];
    size_t begins[p];

    compute_ranges(begins, ends, n, p);
    // Generate random 64 bit integers
    init_rand(a, n);

    const double start = omp_get_wtime();
    for (int shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            const int start = begins[tid];
            const int end = ends[tid];
            size_t *histo = histogram[tid];
            memset(histo, 0, buckets * sizeof(size_t));

            for (int i = start; i < end; ++i)
                ++histo[(a[i] >> shift) & (buckets - 1)];
        }

        int s = 0;
        for (int i = 0; i < buckets; ++i) {
            for (int j = 0; j < p; ++j) {
                const int t = s + histogram[j][i];
                histogram[j][i] = s;
                s = t;
            }
        }
#pragma omp parallel
        {

            const int tid = omp_get_thread_num();
            const int start = begins[tid];
            const int end = ends[tid];
            size_t *histo = histogram[tid];
            for (int i = start; i < end; ++i) {
                ull val = a[i];                         // get value
                int t = (val >> shift) & (buckets - 1); // get bucket
                permuted[++histo[t]] = val;
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
    for (int i = 0; i < p; ++i)
        free(histogram[i]);
    free(histogram);
    return end - start;
}
