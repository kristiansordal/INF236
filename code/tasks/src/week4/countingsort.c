#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {

    int size;
    printf("Enter the size of the array: ");
    scanf("%d", &size);

    int *arr = (int *)malloc(size * sizeof(int));
    int *sorted = (int *)malloc(size * sizeof(int));
    int smaller;

    for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }

    double time = omp_get_wtime();

#pragma omp parallel for schedule(runtime) private(smaller)
    for (int i = 0; i < size; i++) {
        smaller = 0;
        for (int j = 0; j < size; j++) {

            // tie break on smaller index
            if (arr[i] == arr[j] && i < j) {
                smaller++;
            } else if (arr[j] < arr[i]) {
                smaller++;
            }
        }
        sorted[smaller] = arr[i];
    }

    time = omp_get_wtime() - time;

    if (size <= 20) {
        printf("--- Before sorting ---\n");
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n----------------------\n");

        printf("--- After sorting ---\n");
        for (int i = 0; i < size; i++) {
            printf("%d ", sorted[i]);
        }
        printf("\n---------------------\n");
    }

    free(arr);
    free(sorted);

    printf("Sorting %d elements took %f seconds\n", size, time);

    return 0;
}
