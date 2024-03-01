#include "radix_seq.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

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

    const double start = omp_get_wtime();

    for (int shift = 0; shift < BITS; shift += b) {
        int_init(bs, buckets);

        // Get bucket sizes
        for (int i = 0; i < n; i++)
            bs[(a[i] >> shift) & (buckets - 1)]++;

        // Prefix sum
        int s = bs[0];
        bs[0] = 0;
        for (int i = 1; i < buckets; i++) {
            const int t = s + bs[i];
            bs[i] = s;
            s = t;
        }

        // Sort into tmp
        for (int i = 0; i < n; i++) {
            ull val = a[i];                         // get value
            int t = (val >> shift) & (buckets - 1); // get bucket
            permuted[bs[t]++] = val;
        }

        // Swap pointers
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

    const int ans = is_sorted(a, n);
#if defined(SEQ) || defined(COMPARE)
    if (ans == 1)
        printf("SEQUENTIAL: Success!\n");
    else
        printf("SEQUENTIAL: Failure!\n");
#endif
    free(a);
    free(permuted);
    free(bs);
    return end - start;
}
