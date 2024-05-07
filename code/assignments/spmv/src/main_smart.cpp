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
    std::vector<double> A(csr.N, 0), y(csr.N, 0);

    int num_steps = 100;
    double t_start, t_end;
    unsigned long long int ops;

    for (int i = 0; i < csr.N; i++)
        A[i] = ((double)rand() / (RAND_MAX)) + 1;

    std::vector<int> partition_start_indices(k + 1, 0);
    csr.partition_metis(k, partition_start_indices, A);
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
    //     double pc =
    //         (double)(csr.row_ptr[std::get<1>(csr.partition[i])] - csr.row_ptr[std::get<0>(csr.partition[i])]) /
    //         csr.nnz;
    //     pcs[i] = pc;
    //     std::cout << "Rank " << i << " " << pc * 100 << "%" << std::endl;
    // }
    return 0;

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
    }

    t_end = omp_get_wtime();
    ops = 2 * csr.nnz * num_steps; // 2 flops per nnz
    std::cout << "ti_sm " << t_end - t_start << "\n";
    std::cout << "tr_sm " << k << std::endl;
    std::cout << "gf_sm " << ops / ((t_end - t_start) * 1e9) << "\n";
}
