#include "radix_seq.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double radix_sort_seq(int n, int b) {
    ull *a = (ull *)malloc(n * sizeof(ull));
    ull *tmp = (ull *)malloc(n * sizeof(ull));
    const int buckets = 1 << b;
    int bs[buckets];

    // Generate random 64 bit integers
    init_rand(a, n);

    const double ts = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
        int_init(bs, buckets);

        // Get bucket sizes
        for (int i = 0; i < n; i++)
            bs[(a[i] >> shift) & (buckets - 1)]++;

        // Prefix Sum
        for (int i = 1; i < buckets; i++)
            bs[i] += bs[i - 1];

        // Sort into tmp
        for (int i = n - 1; i >= 0; i--) {
            tmp[--bs[(a[i] >> shift) & (buckets - 1)]] = a[i];
        }

        // Swap pointers
        ull *swap = a;
        a = tmp;
        tmp = swap;
    }

    const double te = omp_get_wtime();
    free(tmp);

    if (n <= 20) {
        for (int i = 0; i < n; i++) {
            printf("%llu\n", a[i]);
        }
    }

    const int ans = is_sorted(a, n);
#ifdef SEQ
    if (ans == 1)
        printf("SEQUENTIAL: Success!\n");
    else
        printf("SEQUENTIAL: Failure!\n");
#endif
    free(a);
    return te - ts;
}
