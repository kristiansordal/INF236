#include "week1.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #define STATIC
// #define DYNAMIC
int main(int argc, char **argv) {
    // Task 1
#ifdef STATIC
    static_allocator();
#endif

    // Task 2
#ifdef DYNAMIC
    dynamic_allocator();
#endif

    // Task 3
    int n = 20;
    int *arr = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }

    print_arr(arr, n);
    sort(arr, n);
    print_arr(arr, n);
    printf("\n");

    free(arr);

    int date_count = 5;
    struct Date *dates = (struct Date *)malloc(date_count * sizeof(struct Date));

    for (int i = 0; i < date_count; i++) {
        struct Date date;
        date.year = rand() % 2024;
        date.month = (rand() % 12) + 1;

        // We don't mention february here....
        date.day = (rand() % 31) + 1;
        dates[i] = date;
    }

    for (int i = 0; i < date_count; i++) {
        print_date(&dates[i]);
        printf(" ");
    }

    sort_date(dates, date_count);

    printf("\n");

    for (int i = 0; i < date_count; i++) {
        print_date(&dates[i]);
        printf(" ");
    }

    free(dates);

    // Task 4
    struct Matrix A;
    struct Matrix B;

    printf("\n");
    printf("\n");

    double mat_elem_count = 16;
    double sr = sqrt(mat_elem_count);

    if (sr * sr != mat_elem_count) {
        printf("ERROR: Matrix must be square\n");
        return -1;
    } else {
        init_matrix(&A, (int)mat_elem_count);
        init_matrix(&B, (int)mat_elem_count);
    }

    struct Matrix C = mat_mult(&A, &B);
    print_triangular(&A);
    printf("\n");
    print_triangular(&B);
    printf("\n");
    print_matrix(&C);

    free(A.arr);
    free(B.arr);
    free(C.arr);

    return 0;
}
