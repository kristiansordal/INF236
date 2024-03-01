#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_ranges(int *begins, int *ends, int n, int p) {
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

    int **histogram = (int **)malloc(p * sizeof(int *));
    if (histogram == NULL) {
        printf("Failed to allocate memory for histogram\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < p; i++) {
        histogram[i] = (int *)malloc(buckets * sizeof(int));
        if (histogram[i] == NULL) {
            printf("Failed to allocate memory for histogram[%d]\n", i);
            exit(EXIT_FAILURE);
        }
    }
    // int bs[buckets]; // Bucket size table
    int *bs = (int *)malloc(buckets * sizeof(int));
    if (bs == NULL) {
        printf("Failed to allocate memory for bucket size array\n");
        exit(EXIT_FAILURE);
    }

    int *begins = (int *)malloc(p * sizeof(int));
    if (begins == NULL) {
        printf("Failed to allocate memory for begins\n");
        exit(EXIT_FAILURE);
    }

    int *ends = (int *)malloc(p * sizeof(int));
    if (ends == NULL) {
        printf("Failed to allocate memory for ends\n");
        exit(EXIT_FAILURE);
    }
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

    free(a);
    free(permuted);
    for (int i = 0; i < p; i++)
        free(histogram[i]);
    free(histogram);
    free(bs);
    free(begins);
    free(ends);
    return end - start;
}
