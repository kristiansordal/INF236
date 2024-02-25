#pragma once
double radix_sort_par(int n, int b);
void parallel_inclusive_scan(int *bucket_size, const int NUM_BUCKETS);
void inclusive_scan(int *bucket_size, const int NUM_BUCKETS);
