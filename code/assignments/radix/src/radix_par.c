#include "radix_par.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Parallel radix sort
double radix_sort_par(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *permuted = (ull *)malloc(n * sizeof(ull));
    const int buckets = 1 << b;
    int p;

#pragma omp parallel
    {
#pragma omp critical
        { p = omp_get_num_threads(); }
    }

    int histogram[p][buckets]; // Sub-bucket-size table
    int bs[buckets];           // Bucket size table
    int pfs[buckets];          // Prefix sum table
    ull out_buf[p][buckets];

    // Generate random 64 bit integers
    init_rand_par(a, n);
    const double start = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            int_init_par(histogram[tid], buckets);
            int_init_par(bs, buckets);
            int_init_par(pfs, buckets);

#pragma omp for nowait
            for (int i = 0; i < n; i++)
                histogram[tid][(a[i] >> shift) & (buckets - 1)]++;
        }

        int s = 0;
        for (int i = 0; i < buckets; i++) {
            for (int j = 0; j < p; j++) {
                const int t = s + histogram[j][i];
                histogram[j][i] = s;
                s = t;
            }
            bs[i] = s;
        }

#pragma omp parallel
        {
            const int tid = omp_get_thread_num();
            int *histo_tid = histogram[tid];

#pragma omp for
            for (int i = 0; i < n; i++) {
                ull val = a[i];                         // get value
                int t = (val >> shift) & (buckets - 1); // get bucket
                // ++histo_tid[t];                         // index in permuted
                permuted[histo_tid[t]++] = val;

                // when buffer is full
                // loop through histo tid
                // dist[p++] = my_buf[t][j]
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
    return end - start;
}

// for (int i = n - 1; i >= 0; i--) {
//     int bucket = (a[i] >> shift) & (buckets - 1);
//     permuted[--bs[bucket]] = a[i];
// }
