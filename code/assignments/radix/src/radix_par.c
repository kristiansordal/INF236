#include "radix_par.h"
#include "mt19937-64.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void parallel_inclusive_scan(ull *bucket_size, const int NUM_BUCKETS) {
    int p;
#pragma omp parallel
    {
#pragma omp single
        p = omp_get_num_threads(); // Get the number of threads
    }

    // FALLBACK: If number of buckets is smaller than amount of threads, just use normal PFS
    if (p > NUM_BUCKETS) {
        inclusive_scan(bucket_size, NUM_BUCKETS);
        return;
    }

    ull *sums = malloc(p * NUM_BUCKETS * sizeof(ull));

#pragma omp parallel
    {

        int thread = omp_get_thread_num();
        int start = (thread * NUM_BUCKETS) / p;
        int end = thread == p - 1 ? NUM_BUCKETS : ((thread + 1) * NUM_BUCKETS) / p;

        for (int i = start + 1; i < end; i++) {
            bucket_size[i] += bucket_size[i - 1];
        }
        sums[thread] = bucket_size[end - 1];
    }

    for (int i = 1; i < p; i++) {
        sums[i] += sums[i - 1];
    }

#pragma omp parallel
    {
        int thread = omp_get_thread_num();
        int start = (thread * NUM_BUCKETS) / p;
        int end = thread == p - 1 ? NUM_BUCKETS : ((thread + 1) * NUM_BUCKETS) / p;
        int addend = thread > 0 ? sums[thread - 1] : 0;
        for (int i = start; i < end; i++) {
            if (thread > 0)
                bucket_size[i] += addend;
        }
    }
    free(sums);
}

void inclusive_scan(ull *bucket_size, const int NUM_BUCKETS) {
    for (int i = 1; i < NUM_BUCKETS; i++) {
        bucket_size[i] += bucket_size[i - 1];
    }
}

/* Parallel radix sort
 * TODO Add comments
 *
 */
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *tmp = (ull *)malloc(n * sizeof(ull));

    // Generate random 64 bit integers
    for (int i = 0; i < n; i++) {
        a[i] = genrand64_int64();
    }

    const double start = omp_get_wtime();
    const int NUM_BUCKETS = 1 << b;
    ull bucket_size[NUM_BUCKETS];

    // For each bit in the number, LSD order
    for (int shift = 0; shift < BITS; shift += b) {

#pragma omp parallel for
        for (int i = 0; i < NUM_BUCKETS; i++) {
            bucket_size[i] = 0;
        }

        // Compute the size of each bucket
#pragma omp parallel for reduction(+ : bucket_size[ : NUM_BUCKETS])
        for (int i = 0; i < n; i++) {
            // Right shift value in array[i] - Discards the least significant bits
            // Bitwise and with number of buckets - 1. 16 buckets gives 15 which is 1111 in binary
            int bucket = (a[i] >> shift) & (NUM_BUCKETS - 1);
            bucket_size[bucket]++;
        }

        // Prefix sum to compute the start of each bucket
        parallel_inclusive_scan(bucket_size, NUM_BUCKETS);

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

    if (n <= 20) {
        for (int i = 0; i < n; i++) {
            printf("%llu\n", a[i]);
        }
    }

    int ans = is_sorted(a, n);
    if (ans == 1)
        printf("PARALLEL: Success!\n");
    else
        printf("PARALLEL: Failed!\n");

    free(a);
    return end - start;
}
