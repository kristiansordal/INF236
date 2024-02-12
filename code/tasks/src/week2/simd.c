#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xmmintrin.h>
#define size 4194304

/*
 * This program shows the use of SIMD operations when performing
 * pairwise multiplications of the elements in two vectors.
 */

// Generate size random floats and return a pointer to them

float *Random() {

    unsigned int seed = 123;
    float *t __attribute__((aligned(16))) = malloc(size * sizeof(float));
    //    float *t = malloc(size*sizeof(float));
    int i;
    float num = 0.0;

    for (i = 0; i < size; i++) {
        num = rand() / (RAND_MAX + 1.0);
        t[i] = num;
    }

    return t;
}

float ScalarSSE(float *m1, float *m2) {

    float prod = 0.0;
    int i;
    __m128 X, Y, Z; // Variables of length 128 bits

    Z = _mm_setr_ps(0.0, 0.0, 0.0, 0.0);

    // Process 4 floats at a time
    for (i = 0; i < size; i += 4) {
        X = _mm_load_ps(&m1[i]); // Load 4 floats into X
        Y = _mm_load_ps(&m2[i]); // Load 4 floats into Y
        X = _mm_mul_ps(X, Y);    // Pairwise multiplications, store in X
        Z = _mm_add_ps(X, Z);    // Add the result into Z
    }

    // Sum up the the final result
    for (i = 0; i < 4; i++) {
        prod += Z[i];
    }

    return prod;
}

int main(int argc, char *argv[]) {

    int i;
    time_t start, stop;
    double avg_time = 0;
    double cur_time;
    float *s1, *s2;

    float scalar_product_sse = 0.0;
    for (i = 0; i < 100; i++) {
        s1 = Random();
        s2 = Random();
        start = clock();
        scalar_product_sse += ScalarSSE(s1, s2);
        stop = clock();
        cur_time = ((double)stop - start) / CLOCKS_PER_SEC;
        avg_time += cur_time;
    }
    printf("%f\n", scalar_product_sse);
    printf("SIMD code used on average %f seconds.\n", avg_time / 100);
}
