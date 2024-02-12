#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define n_runs 1
#define BUFF 32 // Higer value gives more random numbers

int main(int argc, char *argv[]) {

    double start = omp_get_wtime();
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
    double end = omp_get_wtime();
    printf("Used %lf seconds \n", end - start);
}
