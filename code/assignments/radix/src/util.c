#include "util.h"
#include "mt19937-64.h"
int is_sorted(ull *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (array[i] > array[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void ull_init(ull *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = 0;
}
void int_init(int *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = 0;
}

void int_init_slice(int *arr, int start, int end) {
    for (int i = start; i < end; i++)
        arr[i] = 0;
}

void init_rand(ull *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = genrand64_int64();
}

void swap(ull *a, ull *b) {
    ull *s = a;
    a = b;
    b = s;
}
