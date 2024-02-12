
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n_runs 1
#define BUFF 1 // Higer value gives more random numbers

static long num_points = 100000000;
struct Point {
    double x, y;
};
int main(int argc, char *argv[]) {

    double pi;
    srand(time(NULL));
    struct Point *points = (struct Point *)malloc(sizeof(struct Point) * num_points);

    for (int i = 0; i < num_points; i++) {
        struct Point p;
        p.x = (double)rand() / RAND_MAX;
        p.y = (double)rand() / RAND_MAX;
        points[i] = p;
    }

    double time = omp_get_wtime();
    double inside = 0;
#pragma omp parallel for reduction(+ : inside)
    for (int i = 0; i < num_points; i++) {
        if (sqrt(points[i].x * points[i].x + points[i].y * points[i].y) <= 1) {
            inside++;
        }
    }

    pi = inside / (double)num_points * 4;
    time = omp_get_wtime() - time;
    printf("Pi: %f\n", pi);
    printf("Time: %f\n", time);

    return 0;
}
