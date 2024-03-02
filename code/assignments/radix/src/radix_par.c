#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CACHE_LINE_SIZE 128

void *aligned_alloc_generic(size_t size, size_t num_elements, size_t element_size) {
    void *block = NULL;
    if (posix_memalign(&block, size, num_elements * element_size) != 0) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return block;
}
void compute_ranges(size_t *begins, size_t *ends, int n, int p) {
    // int avg = n / p;
    begins[0] = 0;
    for (int i = 0; i < p - 1; i++) {
        ends[i] = (i + 1) * (n / p);
        begins[i + 1] = ends[i];
    }
    ends[p - 1] = n;
}

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)aligned_alloc_generic(CACHE_LINE_SIZE, n, sizeof(ull));
    ull *permuted = (ull *)aligned_alloc_generic(CACHE_LINE_SIZE, n, sizeof(ull));

    const int buckets = 1 << b;
    int p = 0;
#pragma omp parallel
    {
#pragma omp master
        { p = omp_get_num_threads(); }
    }

    size_t ends[p];
    size_t begins[p];
    compute_ranges(begins, ends, n, p);

    size_t **histogram = (size_t **)malloc(p * sizeof(size_t *));
    for (int i = 0; i < p; i++)
        histogram[i] = (size_t *)aligned_alloc_generic(CACHE_LINE_SIZE, buckets, sizeof(size_t));

    // Generate random 64 bit integers
    init_rand(a, n);

    const double start = omp_get_wtime();
    for (int shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            memset(histogram[tid], 0, buckets * sizeof(size_t));

            for (int i = begins[tid]; i < ends[tid]; ++i)
                ++histogram[tid][(a[i] >> shift) & (buckets - 1)];
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
            for (int i = begins[tid]; i < ends[tid]; ++i) {
                ull val = a[i];                         // get value
                int t = (val >> shift) & (buckets - 1); // get bucket
                permuted[histogram[tid][t]++] = val;
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

    // Cleanup
    free(a);
    free(permuted);
    for (int i = 0; i < p; ++i)
        free(histogram[i]);
    free(histogram);

    // Return exectuion time;
    return end - start;
}
