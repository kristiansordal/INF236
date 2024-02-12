#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    int a, b, is_prime;
    printf("Enter the range: ");
    scanf("%d %d", &a, &b);
    int *primes = (int *)malloc(sizeof(int) * b);
    int num_primes = 0;
    double time = omp_get_wtime();

#pragma omp parallel for reduction(+ : num_primes)
    for (int i = a; i < b; i++) {
        if (i <= 1)
            continue;
        is_prime = 1;
        for (int j = 2; j <= (int)sqrt(i); j++) {

            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime == 1) {
            {
                primes[num_primes++] = i;
            }
        }
    }
    if (num_primes <= 50) {
        for (int i = 0; i < num_primes; i++) {
            printf("%d ", primes[i]);
        }
        printf("\n");
    }
    time = omp_get_wtime() - time;
    printf("Time to find %d primes: %f\n", num_primes, time);
    return 0;
}
