#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void parallel_inclusive_scan(int *bs, int *sums, const int buckets, int p) {

    // FALLBACK: If number of buckets is smaller than amount of threads, just use normal PFS
    if (p > buckets) {
        printf("Fallback on normal pfs\n");
        inclusive_scan(bs, buckets);
        return;
    }

#pragma omp parallel
    {
        int thread = omp_get_thread_num();
        int start = (thread * buckets) / p;
        int end = thread == p - 1 ? buckets : ((thread + 1) * buckets) / p;

        for (int i = start + 1; i < end; i++)
            bs[i] += bs[i - 1];

        sums[thread] = bs[end - 1];
    }

    for (int i = 1; i < p; i++) {
        sums[i] += sums[i - 1];
    }

#pragma omp parallel
    {
        int thread = omp_get_thread_num();
        int start = (thread * buckets) / p;
        int end = thread == p - 1 ? buckets : ((thread + 1) * buckets) / p;
        int addend = thread > 0 ? sums[thread - 1] : 0;
        for (int i = start; i < end; i++) {
            if (thread > 0)
                bs[i] += addend;

#pragma omp single
            { printf("%d ", bs[i]); }
        }
#pragma omp single
        { printf("\n"); }
    }
}

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *tmp = (ull *)malloc(n * sizeof(ull));
    const int buckets = 1 << b;
    int bs[buckets];
    int p;

#pragma omp parallel
    {
#pragma omp critical
        { p = omp_get_num_threads(); }
    }

    // int lbs[p][buckets];

    // Generate random 64 bit integers
    init_rand_par(a, n);

    const double start = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
        int_init_par(bs, buckets);

#pragma omp parallel
        {
            int lb[buckets];
            int_init_par(lb, buckets);

#pragma omp for
            for (int i = 0; i < n; i++)
                lb[(a[i] >> shift) & (buckets - 1)]++;

#pragma omp critical
            {
                for (int i = 0; i < buckets; i++)
                    bs[i] += lb[i];
            }
        }

        inclusive_scan(bs, buckets);

        // Place the bit sorted elements back into the array
        for (int i = n - 1; i >= 0; i--) {
            tmp[--bs[(a[i] >> shift) & (buckets - 1)]] = a[i];
        }

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

#ifdef PAR
    if (is_sorted(a, n) == 1)
        printf("PARALLEL: Success!\n");
    else
        printf("PARALLEL: Failed!\n");
#endif

    free(a);
    return end - start;
}
