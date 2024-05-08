#pragma once
#include <algorithm>
#include <iostream>
#include <metis.h>
#include <omp.h>
#include <tuple>
#include <vector>

// TODO: Create a struct to store dimensions of matrix, as it is easy to confuse with V and E
template <typename IT, typename VT> class CSR {
  public:
    // N: Number of rows in matrix
    // M: Number of columns in matrix
    int N, M;
    unsigned long long int nnz;
    std::vector<IT> row_ptr, col_idx;
    std::vector<VT> vals;
    std::vector<std::tuple<IT, IT>> partition;

    CSR() = default;
    ~CSR() = default;

    void partition_naive(int p) {
        partition.resize(p);
        int avg_nnz = nnz / p;
        int tid = 0;
        int prev_idx = 0;

        for (int i = 0; i < N; i++) {
            if (tid == p - 1) {
                partition[tid] = std::make_tuple(prev_idx, N);
                break;
            }
            if (row_ptr[i + 1] - row_ptr[prev_idx] > avg_nnz) {
                partition[tid] = std::make_tuple(prev_idx, i + 1); // Adjusted end index
                prev_idx = i + 1;                                  // Move to the next row for the next partition
                tid++;
            }
        }
    }

    /* Partitions a graph into k parts usint METIS_PartGraphRecursive
     * @param k - The number of partitions to make
     * @param start_indices - The partition vector of size |k|+1, where the ith element is the starting index of the ith
     * partition
     * @param A - The input vector for SPMV
     */
    void partition_metis(int k, std::vector<int> &start_indices, std::vector<double> &A) {
        // std::cout << "Starting graph partitioning...\n";
        start_indices[0] = 0;

        if (k == 1) {
            start_indices[1] = N;
            return;
        }

        // value at index i represents which threads node i is assigned to
        std::vector<int> partition_map(N, 0);
        int objval, ncon = 1;

        // represents the allowed unbalance in the partition: 1.01 = 1% unbalance allowed
        real_t ubvec = 1.001;
        // std::vector<int> vwgt(N, 1);
        // std::vector<int> adjw(2 * M, 1);

        int rc = METIS_PartGraphKway(&N, &ncon, row_ptr.data(), col_idx.data(), nullptr, nullptr, nullptr, &k, nullptr,
                                     &ubvec, nullptr, &objval, partition_map.data());

        std::vector<IT> new_id(N, 0), old_id(N, 0);
        int id = 0;

        // reenumerate the vertices
        for (int r = 0; r < k; r++) {
            for (IT i = 0; i < N; i++) {
                if (partition_map[i] == r) {
                    old_id[id] = i;
                    new_id[i] = id++;
                }
            }
            start_indices[r + 1] = id;
        }

        std::vector<IT> new_row_ptr(N + 1, 0);
        std::vector<VT> new_A(N, 0);
        std::vector<IT> new_col_idx(nnz, 0);
        std::vector<VT> new_vals(nnz, 0);

        for (int i = 0; i < N; i++) {
            int degree = row_ptr[old_id[i] + 1] - row_ptr[old_id[i]];
            new_row_ptr[i + 1] = new_row_ptr[i] + degree;

            auto col_start = col_idx.begin() + row_ptr[old_id[i]];
            auto val_start = vals.begin() + row_ptr[old_id[i]];
            std::copy(col_start, col_start + degree, new_col_idx.begin() + new_row_ptr[i]);
            std::copy(val_start, val_start + degree, new_vals.begin() + new_row_ptr[i]);

            for (IT j = new_row_ptr[i]; j < new_row_ptr[i + 1]; j++)
                new_col_idx[j] = new_id[new_col_idx[j]];
        }

        for (int i = 0; i < N; i++)
            new_A[i] = A[new_id[i]];

        partition.resize(k);
        for (int i = 0; i < k; i++)
            partition[i] = std::make_tuple(start_indices[i], start_indices[i + 1]);

        row_ptr = new_row_ptr;
        col_idx = new_col_idx;
        vals = new_vals;
        A = new_A;
        // std::cout << "Graph partitioning done\n";
    }
};
