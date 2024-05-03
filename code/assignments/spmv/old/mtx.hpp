#pragma once
#include <csr.hpp>
#include <mmio.hpp>
#include <omp.h>
#include <tuple>
#include <vector>
template <typename IT, typename VT> class MTX {
  private:
    int N = 0, M = 0, nnz = 0;
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

        int M, N, nz;
        double *val;
        int *I, *J;

        // Call mmio function to read the matrix
        int result = mm_read_unsymmetric_sparse(file_path.c_str(), &M, &N, &nz, &val, &I, &J);
        if (result != 0) {
            std::cerr << "Error reading matrix from file: " << file_path << std::endl;
            return;
        }

        // Resize vector to hold triplets and fill it
        triplets.resize(nz);
        for (int i = 0; i < nz; i++) {
            triplets[i] = std::make_tuple(I[i], J[i], val[i]);
        }

        // Free the dynamically allocated memory from mm_read_unsymmetric_sparse
        free(val);
        free(I);
        free(J);

        N = M; // Assuming square matrix for simplicity
        this->M = M;
        this->N = N;
        nnz = nz;

        std::cout << "|V| = " << N << " |E| = " << nnz << std::endl;
        std::cout << "Done reading MTX file...\n";

        std::cout << "|V| = " << N << " |E| = " << nnz << "\n";
        std::cout << "Done reading MTX file...\n";
    }

    bool read_graph(std::string &file_path, CSR<IT, VT> &csr) {
        FILE *fp;
        long int i;
        int x, y, z;
        double v, w;
        MM_typecode matcode;
        int cs;
        int mxdeg = 0;

        fp = fopen(file_path.c_str(), "r");

        if (fp == NULL) {
            std::cout << "Could not open file " << file_path << "\n";
            return false;
        }

        if (mm_read_banner(fp, &matcode) != 0) {
            std::cout << "Could not process Matrix Market banner.\n";
            return false;
        }

        if ((mm_read_mtx_crd_size(fp, &N, &M, &nnz)) != 0) {
            std::cout << "Could not read size of graph.\n";
            return false;
        }
        std::cout << "N: " << N << " M: " << M << " nnz: " << nnz << std::endl;

        // std::cout << *matcode << std::endl;
        // if (!mm_is_matrix(matcode) || !mm_is_sparse(matcode) || !mm_is_symmetric(matcode)) {
        //     std::cout << "The program can only read files that are sparse symmmetric matrices in coordinate format!
        //     \n"; return false;
        // }

        std::vector<int> count(N, 0);
        triplets.resize(nnz);
        int num_edges = 0;

        // Read in the edges
        if (mm_is_real(matcode)) {
            std::cout << "Real matrix, Starting to read " << M << " edges \n";
            srand48(time(NULL));
            for (i = 0; i < nnz; i++) {
                fscanf(fp, "%d %d %lf", &x, &y, &v); // Use this line if there is exactly one double weight
                if (x != y) {                        // Avoid self-edges
                    count[--x]++;
                    count[--y]++;
                    triplets[num_edges++] = std::make_tuple(x, y, v);
                }
            }
        } else if (mm_is_integer(matcode)) {
            std::cout << "Integer matrix, Starting to read " << nnz << " edges \n";
            srand48(time(NULL));
            for (i = 0; i < nnz; i++) {
                fscanf(fp, "%d %d %lf", &x, &y, &v); // Use this line if there is exactly one double weight
                if (x != y) {                        // Avoid self-edges
                    count[--x]++;
                    count[--y]++;
                    triplets[num_edges++] = std::make_tuple(x, y, v);
                }
            }
        } else { // Symbolic matrix
            srand48(time(NULL));
            // printf("Symbolic matrix \n");
            for (i = 0; i < nnz; i++) {
                fscanf(fp, "%d %d", &x, &y);
                if (x != y) { // Avoid self-edges
                    count[--x]++;
                    count[--y]++;
                    triplets[num_edges++] = std::make_tuple(x, y, 1.0);
                }
            }
        }

        std::cout << "done triplets" << std::endl;
        csr.row_ptr.resize(N + 1, 0);
        csr.col_idx.resize(nnz * 2);
        csr.vals.resize(nnz * 2);
        std::vector<int> offsets(N, 0);
        int sum = 0;
        for (size_t i = 0; i < count.size(); ++i) {
            sum += count[i];
            if (i + 1 < csr.row_ptr.size())
                csr.row_ptr[i + 1] = sum;
        }

        for (auto t : triplets) {
            int v = std::get<0>(t);
            int u = std::get<1>(t);
            double w = std::get<2>(t);
            csr.vals[csr.row_ptr[v] + offsets[v]] = w;
            csr.vals[csr.row_ptr[u] + offsets[u]] = w;
            csr.col_idx[csr.row_ptr[v] + offsets[v]++] = u;
            csr.col_idx[csr.row_ptr[u] + offsets[u]++] = v;
        }

        csr.N = N;
        csr.M = M;
        csr.V = N;
        //  this only applies for symmetrical matrices
        csr.E = 2 * nnz;
        csr.nnz = 2 * nnz;
        return true;
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
