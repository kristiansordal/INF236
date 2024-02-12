#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static long num_steps = 10000000;
double step;

int main(int argc, char **argv) {

    int i;
    double x, pi, sum = 0;
    omp_set_num_threads(4);
    step = 1.0 / (double)num_steps;
    int local_steps = (int)num_steps / 4;
    double *sums = (double *)malloc(sizeof(double) * 4);

#pragma omp parallel
    {
        for (i = omp_get_thread_num() * local_steps; i < (omp_get_thread_num() + 1) * local_steps; i++) {
            x = (i + 0.5) * step;
            sums[omp_get_thread_num()] += 4.0 / (1.0 + x * x);
        }
    }

    for (i = 0; i < 4; i++) {
        sum += sums[i];
    }

    pi = step * sum;

    printf("SUM: %f\n", sum);
    printf("PI: %f\n", pi);
    printf("Time: %f", omp_get_wtime());

    return 0;
}
