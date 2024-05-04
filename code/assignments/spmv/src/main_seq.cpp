#include "spmv.hpp"
#include <csr.hpp>
#include <mtx.hpp>
#include <omp.h>

int main(int argc, char **argv) {
    MTX<int, double> mtx;
    CSR<int, double> csr;
    mtx.parse(argv[1]);
    mtx.mtx_to_csr(csr);
    std::vector<double> A(csr.N, 0), y(csr.N, 0);

    int num_steps = 100;
    double t_start, t_end;
    unsigned long long int ops;

    for (int i = 0; i < csr.N; i++)
        A[i] = ((double)rand() / (RAND_MAX)) + 1;

    t_start = omp_get_wtime();
    for (int i = 0; i < num_steps; i++) {
        spmv(csr, A, y);
        std::swap(A, y);
    }

    t_end = omp_get_wtime();
    ops = 2 * csr.nnz * num_steps; // 2 flops per nnz
    std::cout << "ti_sq " << t_end - t_start << "\n";
    std::cout << "tr_sq " << 0 << std::endl;
    std::cout << "gf_sq " << ops / ((t_end - t_start) * 1e9) << "\n";
}
