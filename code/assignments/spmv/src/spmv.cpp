#include "spmv.hpp"
#include <omp.h>
void spmv(CSR<int, double> &csr, std::vector<double> &A, std::vector<double> &y) {
    for (int v = 0; v < csr.N; v++) {
        double sum = 0;
        for (int u = csr.row_ptr[v]; u < csr.row_ptr[v + 1]; u++)
            sum += csr.vals[u] * A[csr.col_idx[u]];

        y[v] = sum;
    }
}

void spmv_shared(CSR<int, double> &csr, std::vector<double> &A, std::vector<double> &y) {
#pragma omp parallel for schedule(runtime)
    for (int v = 0; v < csr.N; v++) {
        double sum = 0;
        for (int u = csr.row_ptr[v]; u < csr.row_ptr[v + 1]; u++)
            sum += csr.vals[u] * A[csr.col_idx[u]];
        y[v] = sum;
    }
}

void spmv_partition_naive(CSR<int, double> &csr, int &start, int &end, std::vector<double> &A, std::vector<double> &y) {
    for (int v = start; v < end; v++) {
        double sum = 0;
        for (int u = csr.row_ptr[v]; u < csr.row_ptr[v + 1]; u++)
            sum += csr.vals[u] * A[csr.col_idx[u]];
        y[v] = sum;
    }
}
