#include "radix_seq.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double radix_sort_seq(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    if (a == NULL) {
        fprintf(stderr, "Failed to allocate memory for main array\n");
        exit(EXIT_FAILURE);
    }

    ull *permuted = (ull *)malloc(n * sizeof(ull));

    if (permuted == NULL) {
        fprintf(stderr, "Failed to allocate memory for permutation array\n");
        exit(EXIT_FAILURE);
    }

    const int buckets = 1 << b;
    int *bs = (int *)malloc(buckets * sizeof(int));
    if (bs == NULL) {
        printf("Failed to allocate memory for bucket size array\n");
        exit(EXIT_FAILURE);
    }

    // Generate random 64 bit integers
    init_rand(a, n);

    double t, t1 = 0, t2 = 0, t3 = 0;
    const double start = omp_get_wtime();
    double bucket_shift_bench[64 / b];
    double permutation_bench[64 / b];

    int iters = 0;
    for (int shift = 0; shift < BITS; shift += b) { // O(64/b)
        memset(bs, 0, buckets * sizeof(int));       // O(2^b)

        t = omp_get_wtime();
        // Get bucket sizes
        for (int i = 0; i < n; i++)                // O(n)
            bs[(a[i] >> shift) & (buckets - 1)]++; // O(1)
        bucket_shift_bench[iters] = omp_get_wtime() - t;
        t1 += omp_get_wtime() - t;

        // Prefix sum
        t = omp_get_wtime();
        int s = bs[0];
        bs[0] = 0;
        for (int i = 1; i < buckets; i++) { // O(2^b)
            const int t = s + bs[i];        // O(1)
            bs[i] = s;                      // O(1)
            s = t;                          // O(1)
        }
        t2 += omp_get_wtime() - t;

        t = omp_get_wtime();
        // Sort into tmp
        for (int i = 0; i < n; i++) {               // O(n)
            ull val = a[i];                         // O(1)
            int t = (val >> shift) & (buckets - 1); // O(1)
            permuted[bs[t]++] = val;                // O(1)
        }
        permutation_bench[iters++] = omp_get_wtime() - t;
        t3 += omp_get_wtime() - t;

        swap(&a, &permuted);
    }

    const double end = omp_get_wtime();

    if (n <= 20) {
        for (int i = 0; i < n; i++) {
            printf("%llu\n", a[i]);
        }
    }

    const int ans = is_sorted(a, n);
#if defined(SEQ) || defined(COMPARE)
    if (ans == 1)
        printf("SEQUENTIAL: Success!\n");
    else
        printf("SEQUENTIAL: Failure!\n");
#endif
    double bucket_shift_avg, permutation_avg;

    for (int i = 0; i < iters; i++) {
        bucket_shift_avg += bucket_shift_bench[i];
        permutation_avg += permutation_bench[i];
    }

    bucket_shift_avg /= iters;
    permutation_avg /= iters;

    printf("n: %d, b: %d, n/b: %d, it: %d\n", n, b, n / b, iters);
    printf("Bucket shift avg: %f\n", bucket_shift_avg);
    printf("Permutation avg: %f\n", permutation_avg);
    printf("\n");

    printf("Time taken for bucket sizes: %f\n", t1);
    printf("Time taken for prefix sum: %f\n", t2);
    printf("Time taken for sorting: %f\n", t3);
    free(a);
    free(permuted);
    free(bs);
    return end - start;
}
