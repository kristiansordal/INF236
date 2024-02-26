#pragma once
double radix_sort_par(int n, int b);
void parallel_inclusive_scan(int *bs, int *sums, const int buckets, int p);
void inclusive_scan(int *bs, const int buckets);
