#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {

    int size;
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    int *n = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        n[i] = rand() % size;
    }

    int smallest, position, tmp;

    double time = omp_get_wtime();

    for (int i = 0; i < size - 1; i++) {
        smallest = n[i];
        position = i;

#pragma omp parallel for
        for (int j = i + 1; j < size; j++) {
            if (n[j] < smallest) {
#pragma omp critical
                {
                    if (n[j] < smallest) {
                        smallest = n[j];
                        position = j;
                    }
                }
            }
        }

        tmp = n[i];
        n[i] = n[position];
        n[position] = tmp;
    }

    time = omp_get_wtime() - time;
    printf("Time: %f\n", time);

    return 0;
}
