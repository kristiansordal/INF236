#include <fstream>
#include <io.hpp>
#include <omp.h>
int main(int argc, char **argv) {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    MTX<int, double> io;
    CSR<int, double> csr;
    std::string filename = argv[1];
    std::ifstream file(filename);
    double start, end;
    start = omp_get_wtime();
    io.parse(argv[1]);

    io.mtx_to_csr(csr);
    end = omp_get_wtime();
    std::cout << "Time to parse and convert to mtx: " << end - start << std::endl;

    return 0;
}
