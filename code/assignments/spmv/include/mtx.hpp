#pragma once
#include <csr.hpp>
#include <fstream>
#include <omp.h>
#include <sstream>
#include <tuple>
#include <vector>
template <typename IT, typename VT> class MTX {
  private:
    int N, M, nnz;
    std::vector<IT> rows, cols;
    std::vector<VT> vals;
    std::vector<std::tuple<IT, IT, VT>> triplets;

  public:
    MTX() = default;
    ~MTX() = default;

    /* Reads a .mtx file and stores the values in the triplet vector
     * @param file_path: path to the .mtx file
     */
    void read_mtx(const std::string &file_path) {
        std::cout << "Reading MTX file: " << file_path << "\n";
        std::ifstream file(file_path);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file\n";
            return;
        }

        // skip header
        // more robust if we parse it to determine graph type
        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == '%')
                continue;
            break;
        }

        std::istringstream iss(line);
        iss >> N >> M >> nnz;

        triplets.resize(nnz);
        IT row, col;
        VT val;

        int i = 0;
        while (file >> row >> col >> val)
            triplets[i++] = std::make_tuple(row, col, val);

        file.close();

        std::cout << "|V| = " << N << " |E| = " << nnz << "\n";
        std::cout << "Done reading MTX file...\n";
    }

    double l2_norm_triplet() {
        double norm = 0;
        for (int i = 0; i < nnz; i++)
            norm += std::get<2>(triplets[i]) * std::get<2>(triplets[i]);
        return sqrt(norm);
    }

    /* Converts the triplet vector to CSR format
     * @return: Graph object in CSR format
     */
    CSR<IT, VT> mtx_to_csr() {
        std::cout << "Converting from MTX to CSR...\n";
        CSR<IT, VT> graph{};
        std::vector<IT> row_count(N + 1, 0);
        graph.row_ptr.resize(N + 1);
        graph.col_idx.resize(nnz);
        graph.vals.resize(nnz);
        std::cout << "sorting..." << std::endl;

        std::sort(triplets.begin(), triplets.end(),
                  [](const std::tuple<IT, IT, VT> &a, const std::tuple<IT, IT, VT> &b) {
                      if (std::get<0>(a) == std::get<0>(b))
                          return std::get<1>(a) < std::get<1>(b);
                      return std::get<0>(a) < std::get<0>(b);
                  });
        std::cout << "done sorting.." << std::endl;
        for (int i = 0; i < nnz; i++) {
            auto triplet = triplets[i];
            row_count[std::get<0>(triplet)]++;
            graph.col_idx[i] = std::get<1>(triplet);
            graph.vals[i] = std::get<2>(triplet);
        }

        std::cout << "scan" << std::endl;
        int sum = 0;
        for (size_t i = 0; i < row_count.size(); ++i) {
            sum += row_count[i];
            if (i + 1 < graph.row_ptr.size()) {
                graph.row_ptr[i + 1] = sum;
            }
        }
        std::cout << "scan done" << std::endl;
        graph.N = N;
        graph.M = M;
        graph.V = N;
        graph.E = nnz;
        graph.nnz = nnz;
        std::cout << "Done converting from MTX to CSR...\n";
        return graph;
    }
};
