#pragma once
#include <csr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct MatrixType {
    char object, format, field, symmetry;
    bool is_matrix() { return object == 'm'; }
    bool is_coordinate() { return format == 'c'; }
    bool is_symmetric() { return symmetry == 's'; }
    bool is_complex() { return field == 'c'; }
    void print() {
        std::cout << "Object: " << object << "\n";
        std::cout << "Format: " << format << "\n";
        std::cout << "Field: " << field << "\n";
        std::cout << "Symmetry: " << symmetry << "\n";
    }
};

template <typename IT, typename VT> class MTX {
  private:
    int N, M, nnz;
    std::vector<std::tuple<IT, IT, VT>> triplets;
    std::vector<IT> counts;
    void parse_banner(std::ifstream &file) {
        // std::cout << "Reading Banner\n";
        std::string line;
        std::string word;
        std::vector<std::string> tokens;

        // parse matrix type
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            while (iss >> word)
                tokens.push_back(word);

            type.object = tokens[1][0];
            type.format = tokens[2][0];
            type.field = tokens[3][0];
            type.symmetry = tokens[4][0];

            tokens.clear();
        }

        if (!type.is_matrix()) {
            std::cerr << "MatrixMarket object " << type.object << " not supported!\n";
            return;
        }

        if (!type.is_coordinate()) {
            std::cerr << "Only coordinate format matrices are supported!\n";
            return;
        }
        if (type.is_complex()) {
            std::cerr << "Complex valued matrices are not supported!\n";
            return;
        }

        // parse rest of information until we encounter the dimensions
        while (std::getline(file, line))
            if (line[0] != '%') {
                std::istringstream iss(line);
                while (iss >> word)
                    tokens.push_back(word);
                N = std::stoi(tokens[0]);
                M = std::stoi(tokens[1]);
                nnz = std::stoi(tokens[2]);
                triplets.resize(nnz);
                counts.resize(N);
                break;
            }

        // std::cout << "Done reading banner\n";
    }

    // Parses a symmetric coordinate matrix
    void parse_symmetric_coordinate_matrix(std::ifstream &file) {
        int num_read = 0;
        std::string line;
        std::istringstream iss;
        IT v, u;
        int edges = 0;
        while (std::getline(file, line)) {
            num_read++;

            // if (num_read % 100000 == 0) {
            //     std::cout << num_read << " of " << nnz << std::endl;
            // }
            std::istringstream iss(line);
            iss >> v >> u;
            triplets[edges++] = std::make_tuple(--v, --u, 1.0);
            counts[v]++;
            if (v != u)
                counts[u]++;
        }
    }

    // Parses a general coordinate matrix
    void parse_general_coordinate_matrix(std::ifstream &file) {
        int num_read = 0;
        std::string line;
        std::istringstream iss;
        IT v, u;
        int edges = 0;
        while (std::getline(file, line)) {
            num_read++;
            // if (num_read % 100000 == 0) {
            //     std::cout << num_read << " of " << nnz << std::endl;
            // }
            std::istringstream iss(line);
            iss >> v >> u;
            triplets[edges++] = std::make_tuple(--v, --u, 1.0);
            counts[v]++;
        }
    }

    // Parses a symmetric matrix with nonzero entries
    void parse_symmetric_matrix(std::ifstream &file) {
        std::string line;
        int num_read = 0;
        IT v, u;
        VT w;
        int edges = 0;
        while (std::getline(file, line)) {

            num_read++;
            // if (num_read % 100000 == 0) {
            //     std::cout << num_read << " of " << nnz << std::endl;
            // }
            std::istringstream iss(line);
            iss >> v >> u >> w;
            triplets[edges++] = std::make_tuple(--v, --u, w);
            counts[v]++;
            if (v != u)
                counts[u]++;
        }
    }

    // Parses a general matrix with nonzero entries
    void parse_general_matrix(std::ifstream &file) {
        std::string line;
        int num_read = 0;
        IT v, u;
        VT w;
        int edges = 0;
        while (std::getline(file, line)) {

            num_read++;
            // if (num_read % 100000 == 0) {
            //     std::cout << num_read << " of " << nnz << std::endl;
            // }
            std::istringstream iss(line);
            iss >> v >> u >> w;
            triplets[edges++] = std::make_tuple(--v, --u, w);
            counts[v]++;
        }
    }

  public:
    MTX() = default;
    ~MTX() = default;
    MatrixType type;

    void parse(const std::string &filename) {
        std::ifstream file(filename);
        parse_banner(file);
        // type.print();

        // std::cout << "Starting reading triplet" << std::endl;
        if (type.field == 'p')
            if (type.symmetry == 's')
                parse_symmetric_coordinate_matrix(file);
            else
                parse_general_coordinate_matrix(file);
        else {
            if (type.symmetry == 's')
                parse_symmetric_matrix(file);
            else
                parse_general_matrix(file);
        }
        // std::cout << "Done reading matrix" << std::endl;
        file.close();
    }

    void mtx_to_csr(CSR<IT, VT> &csr) {
        // std::cout << "Converting to mtx" << std::endl;
        csr.N = N;
        csr.M = M;
        csr.nnz = nnz;

        if (type.symmetry == 's')
            csr.nnz *= 2;
        csr.row_ptr.resize(N + 1);
        csr.col_idx.resize(csr.nnz);
        csr.vals.resize(csr.nnz);

        std::vector<int> offsets(N, 0);
        int sum = 0;
        for (size_t i = 0; i < N; ++i) {
            csr.row_ptr[i] = sum;
            sum += counts[i];
        }
        csr.row_ptr[csr.N] = csr.nnz;

        for (auto t : triplets) {
            int v = std::get<0>(t);
            int u = std::get<1>(t);
            double w = std::get<2>(t);
            csr.vals[csr.row_ptr[v] + offsets[v]] = w;
            csr.col_idx[csr.row_ptr[v] + offsets[v]++] = u;
            if (type.is_symmetric() && v != u) {
                csr.vals[csr.row_ptr[u] + offsets[u]] = w;
                csr.col_idx[csr.row_ptr[u] + offsets[u]++] = v;
            }
        }
        // std::cout << "Done converting to mtx" << std::endl;
    }
};
