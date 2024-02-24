#include "radix_seq.h"
#include "mt19937-64.h"
#include "util.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double radix_sort_seq(int n, int b) {
    ull *array = (ull *)malloc(n * sizeof(ull));
    ull *tmp = (ull *)malloc(n * sizeof(ull));

    // Generate random 64 bit integers
    for (int i = 0; i < n; i++)
        array[i] = genrand64_int64();

    const double start = omp_get_wtime();
    const int NUM_BUCKETS = 1 << b;

    // For each bit in the number, LSD order
    for (int shift = 0; shift < BITS; shift += b) {
        int bucket_size[NUM_BUCKETS];

        for (int i = 0; i < NUM_BUCKETS; i++)
            bucket_size[i] = 0;

        // For each element in the array, put it into the bucket corresponding to its nth bit
        for (int i = 0; i < n; i++) {
            int bucket = (array[i] >> shift) & (NUM_BUCKETS - 1);
            bucket_size[bucket]++;
        }

        // Prefix sum on the bucket sizes
        for (int i = 1; i < NUM_BUCKETS; i++)
            bucket_size[i] += bucket_size[i - 1];

        for (int i = n - 1; i >= 0; i--) {
            int bucket = (array[i] >> shift) & (NUM_BUCKETS - 1);
            tmp[--bucket_size[bucket]] = array[i];
        }

        for (int i = 0; i < n; i++)
            array[i] = tmp[i];
    }

    const double end = omp_get_wtime();
    free(tmp);

    const int ans = is_sorted(array, n);
    if (ans == 1)
        printf("SEQUENTIAL: Success!\n");
    else
        printf("SEQUENTIAL: Failure!\n");

    free(array);

    return end - start;
}
