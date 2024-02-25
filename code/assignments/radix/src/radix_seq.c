#include "radix_seq.h"
#include "mt19937-64.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double radix_sort_seq(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *tmp = (ull *)malloc(n * sizeof(ull));

    double t1;
    double pfs_time = 0;

    // Generate random 64 bit integers
    for (int i = 0; i < n; i++)
        a[i] = genrand64_int64();

    const double start = omp_get_wtime();
    const int NUM_BUCKETS = 1 << b;

    // For each bit in the number, LSD order
    for (int shift = 0; shift < BITS; shift += b) {
        int bucket_size[NUM_BUCKETS];

        for (int i = 0; i < NUM_BUCKETS; i++)
            bucket_size[i] = 0;

        // For each element in the array, put it into the bucket corresponding to its nth bit
        for (int i = 0; i < n; i++) {
            int bucket = (a[i] >> shift) & (NUM_BUCKETS - 1);
            bucket_size[bucket]++;
        }

        // Prefix sum on the bucket sizes
        t1 = omp_get_wtime();
        for (int i = 1; i < NUM_BUCKETS; i++)
            bucket_size[i] += bucket_size[i - 1];
        t1 = omp_get_wtime() - t1;
        pfs_time += t1;

        for (int i = n - 1; i >= 0; i--) {
            int bucket = (a[i] >> shift) & (NUM_BUCKETS - 1);
            tmp[--bucket_size[bucket]] = a[i];
        }

        ull *swap = a;
        a = tmp;
        tmp = swap;
    }

    const double end = omp_get_wtime();
    free(tmp);

    printf("PFS time: %f\n", pfs_time);

    if (n <= 20) {
        for (int i = 0; i < n; i++) {
            printf("%llu\n", a[i]);
        }
    }

    const int ans = is_sorted(a, n);
    if (ans == 1)
        printf("SEQUENTIAL: Success!\n");
    else
        printf("SEQUENTIAL: Failure!\n");

    free(a);

    return end - start;
}
