#include "radix_par.h"
#include "mt19937-64.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void parallel_inclusive_scan(int *bucket_size, int *sums, const int NUM_BUCKETS, int p) {

    // FALLBACK: If number of buckets is smaller than amount of threads, just use normal PFS
    if (p > NUM_BUCKETS) {
        printf("Fallback on normal pfs\n");
        inclusive_scan(bucket_size, NUM_BUCKETS);
        return;
    }

    // ull *sums = malloc(p * sizeof(ull));

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
}

void inclusive_scan(int *bucket_size, const int NUM_BUCKETS) {
    for (int i = 1; i < NUM_BUCKETS; i++) {
        bucket_size[i] += bucket_size[i - 1];
    }
}

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *tmp = (ull *)malloc(n * sizeof(ull));
    double pfs_time = 0, tmp_time = 0, bucket_time = 0;
    double t1, t2, t3;
    int p;

#pragma omp parallel
    {
#pragma omp critical
        { p = omp_get_num_threads(); }
    }

    int *sums = malloc(p * sizeof(int));

    // Generate random 64 bit integers
    for (int i = 0; i < n; i++) {
        a[i] = genrand64_int64();
    }

    const double start = omp_get_wtime();
    const int NUM_BUCKETS = 1 << b;
    int bucket_size[NUM_BUCKETS];

    // For each bit in the number, LSD order
    for (int shift = 0; shift < BITS; shift += b) {

        t3 = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < NUM_BUCKETS; i++) {
            bucket_size[i] = 0;
        }

#pragma omp parallel
        {
            int local_bucket_size[NUM_BUCKETS];
            for (int i = 0; i < NUM_BUCKETS; i++) {
                local_bucket_size[i] = 0;
            }

#pragma omp for nowait
            for (int i = 0; i < n; i++) {
                int bucket = (a[i] >> shift) & (NUM_BUCKETS - 1);
                local_bucket_size[bucket]++;
            }

#pragma omp critical
            {
                for (int i = 0; i < NUM_BUCKETS; i++) {
                    bucket_size[i] += local_bucket_size[i];
                }
            }
        }
        t3 = omp_get_wtime() - t3;
        bucket_time += t3;

        // Prefix sum to compute the start of each bucket
        t1 = omp_get_wtime();
        parallel_inclusive_scan(bucket_size, sums, NUM_BUCKETS, p); // somehow slower than sequential pfs?
        t1 = omp_get_wtime() - t1;
        pfs_time += t1;

        t2 = omp_get_wtime();
        for (int i = n - 1; i >= 0; i--) {
            int bucket = (a[i] >> shift) & (NUM_BUCKETS - 1);
            tmp[--bucket_size[bucket]] = a[i];
        }
        t2 = omp_get_wtime() - t2;
        tmp_time += t2;

        ull *swap = a;
        a = tmp;
        tmp = swap;
    }

    const double end = omp_get_wtime();

    if (n <= 20) {
        for (int i = 0; i < n; i++) {
            printf("%llu\n", a[i]);
        }
    }

    printf("PFS TIME: %f\n", pfs_time);
    printf("TMP TIME: %f\n", tmp_time);
    printf("BUCKET TIME: %f\n", bucket_time);
    int ans = is_sorted(a, n);
    if (ans == 1)
        printf("PARALLEL: Success!\n");
    else
        printf("PARALLEL: Failed!\n");

    free(a);
    return end - start;
}
