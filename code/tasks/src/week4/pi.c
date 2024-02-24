#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n_runs 1
#define BUFF 256

static long long num_points = 1000000000;

int main(int argc, char *argv[]) {
    double pi;
    double inside = 0;
    double t = omp_get_wtime();
    unsigned int seed;

#pragma omp parallel private(seed)
    {
        seed = time(NULL) + omp_get_thread_num(); // Each thread gets a different seed

#pragma omp for reduction(+ : inside)
        for (int i = 0; i < num_points; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            if (sqrt(x * x + y * y) <= 1) {
                inside++;
            }
        }
    }

    pi = 4 * (double)inside / (double)num_points;
    t = omp_get_wtime() - t;

    printf("Pi: %f\n", pi);
    printf("Time: %f\n", t);

    return 0;
}
