#pragma once
#include "util.h"
double radix_sort_par(int n, int b);
void parallel_inclusive_scan(ull *bucket_size, const int NUM_BUCKETS);
void inclusive_scan(ull *bucket_size, const int NUM_BUCKETS);
