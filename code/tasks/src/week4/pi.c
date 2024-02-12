#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n_runs 1
#define BUFF 256 // Adjusted buffer size for better random number generation

static long num_points = 1000000000;

int main(int argc, char *argv[]) {
    double pi;
    double inside = 0;
    double t = omp_get_wtime();

#pragma omp parallel
    {
        unsigned int seed = time(NULL) + omp_get_thread_num();

        for (int i = 0; i < num_points / omp_get_num_threads(); i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            if (sqrt(x * x + y * y) <= 1) {
#pragma omp atomic
                inside++;
            }
        }
    }

    pi = inside / (double)num_points * 4;
    t = omp_get_wtime() - t;
    printf("Pi: %f\n", pi);
    printf("Time: %f\n", t);

    return 0;
}
