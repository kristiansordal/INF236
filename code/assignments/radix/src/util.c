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

void inclusive_scan(int *arr, const int n) {
    for (int i = 1; i < n; i++) {
        arr[i] += arr[i - 1];
    }
}

void init_rand(ull *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = genrand64_int64();
}
