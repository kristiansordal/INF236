#include "spmv.hpp"
#include <csr.hpp>
#include <mtx.hpp>
#include <omp.h>

int main(int argc, char **argv) {
    MTX<int, double> mtx;
    CSR<int, double> csr;
    int k = 0;
#pragma omp parallel
    {
#pragma omp single
        { k = omp_get_num_threads(); }
    }
    mtx.parse(argv[1]);
    mtx.mtx_to_csr(csr);
    csr.partition_naive(k);

    int num_assigned = 0;
    for (auto i : csr.partition) {
        num_assigned += std::get<1>(i) - std::get<0>(i);
    }

    double mn = 1000, mx = 0;
    for (int i = 0; i < k; i++) {
        mn = std::min(
            mn, ((double)(csr.row_ptr[std::get<1>(csr.partition[i])] - csr.row_ptr[std::get<0>(csr.partition[i])]) /
                 csr.nnz) *
                    100);
        mx = std::max(
            mx, ((double)(csr.row_ptr[std::get<1>(csr.partition[i])] - csr.row_ptr[std::get<0>(csr.partition[i])]) /
                 csr.nnz) *
                    100);
    }

    std::cout << "Min " << mn << "\n";
    std::cout << "Max " << mx << "\n";
    std::cout << "Diff " << mx - mn << "\n";
    // std::vector<double> pcs(csr.partition.size(), 0);
    // for (int i = 0; i < csr.partition.size(); i++) {
    //     double pc = (double)(std::get<1>(csr.partition[i]) - std::get<0>(csr.partition[i])) / csr.N;
    //     pcs[i] = pc;
    //     std::cout << "Rank " << i << " " << pc * 100 << "%" << std::endl;
    // }
    return 0;
    if (num_assigned != csr.N) {
        std::cout << "Error, not all nodes have been assigned" << std::endl;
    }
    std::vector<double> A(csr.N, 0), y(csr.N, 0);
    int num_steps = 100;
    double t_start, t_end;
    unsigned long long int ops;

    for (int i = 0; i < csr.N; i++)
        A[i] = ((double)rand() / (RAND_MAX)) + 1;

    t_start = omp_get_wtime();
#pragma omp parallel
    {
        int start = std::get<0>(csr.partition[omp_get_thread_num()]);
        int end = std::get<1>(csr.partition[omp_get_thread_num()]);

        for (int i = 0; i < num_steps; i++) {
            spmv_partition_naive(csr, start, end, A, y);

#pragma omp barrier
#pragma omp master
            { std::swap(A, y); }
#pragma omp barrier
        }
#pragma omp barrier
    }

    t_end = omp_get_wtime();
    ops = 2 * csr.nnz * num_steps; // 2 flops per nnz
    std::cout << "ti_na " << t_end - t_start << "\n";
    std::cout << "tr_na " << k << std::endl;
    std::cout << "gf_na " << ops / ((t_end - t_start) * 1e9) << "\n";
}
