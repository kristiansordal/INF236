#include "week1.h"
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
    struct Date *dates = (struct Date *)malloc(n * sizeof(struct Date));

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
    printf("\n");

    for (int i = 0; i < date_count; i++) {
        print_date(&dates[i]);
        printf(" ");
    }

    free(dates);

    struct Matrix A;
    struct Matrix B;

    printf("\n");

    init_matrix(&A, 4);
    init_matrix(&B, 4);

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
