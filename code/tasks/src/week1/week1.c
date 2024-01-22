#include "week1.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *  STATIC ALLOCATION: int arr[num];
 *  DYNAMIC ALLOCATION: int *arr = (int *)malloc(num * sizeof(int)))
 *                      ^ Same for double, char, float, long etc.
 */

/** Do manual binary search and check if your program compiles or not.
 * Allocation size will vary depening on your system (Brake, Lyng or locally).
 * Also, any print statements etc, will take up memory and reduce the maximum size
 */
void static_allocator() { int arr[2091766]; }

/**
 * We can utilize binary search to find the maximum allowed dynamic allocation size.
 * This is because malloc throws a nullptr if the allocation fails.
 */
void dynamic_allocator() {

    // Use shorthand for long long.
    typedef long long ll;

    ll lo = 0;
    ll hi = ULLONG_MAX / sizeof(int);
    ll mid;
    ll max_allocation_size = 0;
    int *arr;

    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        arr = (int *)malloc(mid * sizeof(int));

        if (arr == NULL) {
            hi = mid - 1;
        } else {
            max_allocation_size = mid;
            lo = mid + 1;
        }
        free(arr);
    }

    // Sanity check, if this allocation doesn't return NULL, we have not found the maximum size.
    arr = malloc((max_allocation_size + 1) * sizeof(int));
    if (arr == NULL) {
        printf("Maximum Dynamic Allocation Size: %llu integers (%llu bytes)\n", max_allocation_size,
               max_allocation_size * sizeof(int));
    }

    free(arr);
}

/**
 * Swap two integers given their memory address.
 *
 * *x means we dereference the memory address and get the value stored at this address.
 * Given x = 1 and y = 2, we have
 *
 *     *x += *y      -> *x = 3
 *     *y = *x - *y  -> *y = 1
 *     *x = *x - *y  -> *x = 2
 *
 * @param x - Pointer to integer x
 * @param y - Pointer to integer y
 */
void swap(int *x, int *y) {
    if (x == y) {
        return;
    }

    *x += *y;
    *y = *x - *y;
    *x = *x - *y;
}

/**
 * Sorts the array using bubble sort in-place
 *
 * @param arr - Pointer to array
 * @param n   - Number of elements in array
 */
void sort(int *arr, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                swap(arr + i, arr + j);
            }
        }
    }
}

/** Check if date x occurs later than date y
 *
 * @param x - Pointer to date x
 * @param y - Pointer to date y
 */
int is_later(struct Date *x, struct Date *y) {
    if (x == y) {
        return false;
    }

    if (x->year != y->year) {
        return x->year > y->year;
    }

    if (x->month != y->month) {
        return x->month > y->month;
    }
    return x->day > y->day;
}

/** Swaps two dates using our previously defined swap function
 *
 * @param x - Pointer to date x
 * @param y - Pointer to date y
 */
void swap_date(struct Date *x, struct Date *y) {
    if (x == y) {
        return;
    }

    swap(&x->year, &y->year);
    swap(&x->month, &y->month);
    swap(&x->day, &y->day);
}

/** Sorts an array using bubble sort
 *
 * @param arr - Pointer to array
 * @param n   - Number of elements in array
 */
void sort_date(struct Date *arr, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (is_later(arr + i, arr + j)) {
                swap_date(arr + i, arr + j);
            }
        }
    }
}

/**
 * Initializes a matrix with index values
 *
 * @param m - Pointer to matrix
 * @param n - Number of elements in matrix
 */
void init_matrix(struct Matrix *m, int n) {
    int rows = sqrt(n);

    m->rows = rows;
    int elems = (rows * (rows + 1)) / 2;
    m->arr = (int *)malloc(elems * sizeof(int));
    m->size = elems;

    for (size_t i = 0; i < elems; i++) {
        m->arr[i] = i + 1;
    }
}

/**
 * Matrix multiplication of symmetrical lower triangular matrices
 *
 * @param A Pointer to matrix A
 * @param B Pointer to matrix B
 */
struct Matrix mat_mult(struct Matrix *A, struct Matrix *B) {
    struct Matrix C;

    if (A->rows != B->rows) {
        printf("Matrices must be of the same size\n");
        return C;
    }

    int n = A->rows;
    C.size = n * n;
    C.rows = n;
    C.arr = (int *)malloc(C.size * sizeof(int));

    /**
     * Sum of natrual numbers: n * (n + 1) / 2
     *
     * As our matrix is symmetrical, we only need to store the lower triangular version.
     * We also store it contiguously in memory, i.e. as a 1D array.
     *
     * To access index (i,j), of a matrix (non-triangular) with row size n, which is stored sequentially in memory, we
     * have A(i,j) = A[i * n + j]
     *
     * Row i has i + 1 elements
     * 1
     * 2 3
     * 4 5 6
     * 7 8 9 10
     *
     * As we have the propery that A[i,j] = A[j,i], we access the elements in the
     * matrices in the following manner:
     *
     * i = the index of the current row
     * j = the index of the current column
     * k = the index of the current element - i.e. the offset
     *
     * Then the element we are interested in is stored at the following index
     * i >= k -> i * (i + 1) / 2 + k
     * i < k  -> k * (k + 1) / 2 + i
     */
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            int sum = 0;

            for (size_t k = 0; k < n; k++) {
                int a_val = (i >= k) ? A->arr[i * (i + 1) / 2 + k] : A->arr[k * (k + 1) / 2 + i];
                int b_val = (j >= k) ? B->arr[j * (j + 1) / 2 + k] : B->arr[k * (k + 1) / 2 + j];
                sum += a_val * b_val;
            }

            C.arr[i * n + j] = sum;
        }
    }
    return C;
}

// Various print functions
void print_arr(int *arr, int n) {
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_date(struct Date *d) { printf("%d/%d/%d", d->day, d->month, d->year); }

void print_matrix(struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->rows; j++) {
            printf("%d ", m->arr[i * m->rows + j]);
        }
        printf("\n");
    }
}

void print_triangular(struct Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->rows; j++) {
            if (i >= j) {
                printf("%d ", m->arr[i * (i + 1) / 2 + j]);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}
