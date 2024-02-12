
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

#pragma omp parallel
    {
        int id = omp_get_thread_num();

        // Set up private data structures for generating random numbers
        struct random_data *rand_state;
        char *rand_statebufs;
        rand_state = (struct random_data *)calloc(1, sizeof(struct random_data));
        rand_statebufs = (char *)calloc(1, BUFF);
        initstate_r(random(), &rand_statebufs[0], BUFF, rand_state);

        //      srandom_r(id+1,rand_state);                 // Use this line to get the same random numbers for each run
        srandom_r(time(NULL) + id + 1, rand_state); // Use this line to get new random numbers for each run

        // Generate "size" random numbers
        //
        int i;
        int size = 1000000;
        int my_buf[size];
        int my_r_num;

        for (i = 0; i < size; i++) {
            random_r(rand_state, &my_r_num); // Generate a random number
            my_buf[i] = my_r_num;
        }

        //      printf("Thread %d has random number %d \n",id,my_r_num);
    }
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
