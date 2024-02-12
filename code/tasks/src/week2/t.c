#include <omp.h>
#include <stdio.h>
int main(int argc, char **argv) {

#pragma omp parallel
    printf("Hello, world, from thread no: %d \n", omp_get_thread_num());

#pragma omp single
    printf("Hello, world, from thread no: %d \n", omp_get_thread_num());

    return 0;
}
