#pragma once
#include <stdbool.h>
void static_allocator();
void dynamic_allocator();
void swap(int *x, int *y);
void sort(int *arr, int n);

struct Date {
    int day;
    int month;
    int year;
};

void sort_date(struct Date *arr, int n);
int is_later(struct Date *x, struct Date *y);
void swap_date(struct Date *x, struct Date *y);

struct Matrix {
    int *arr;
    int rows;
    int size;
};

void init_matrix(struct Matrix *m, int n);
struct Matrix mat_mult(struct Matrix *A, struct Matrix *B);

void print_arr(int *arr, int n);
void print_date(struct Date *d);
void print_matrix(struct Matrix *m);
