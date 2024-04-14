#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define dim 100

int main(int argc, char **argv) {

    int myrank;
    int np;

    double start, end;

    double A[dim][dim];
    double B[dim][dim];
    double C[dim][dim];
    double D[dim][dim];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    // Set up data localy on each process
    // srand48(myrank);
    srand(myrank);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0.0;
        }
    }

    // Main loop

    start = MPI_Wtime();
    for (int i = 0; i < np; i++) {
        printf("it = %d \n", i);

        // Matrix multiplication
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
                for (int k = 0; k < dim; k++)
                    C[i][j] += A[i][k] * B[k][j];

        MPI_Request request;
        // Send B to the next process, receive in D
        printf("Sending \n");
        MPI_Isend(B, dim * dim, MPI_DOUBLE, (myrank + 1) % np, 10, MPI_COMM_WORLD, &request);
        printf("Receiving \n");
        MPI_Irecv(D, dim * dim, MPI_DOUBLE, (myrank - 1 + np) % np, 10, MPI_COMM_WORLD, &request);

        printf("Copying \n");
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
                B[i][j] = D[i][j];
    }
    end = MPI_Wtime();

    if (myrank == 0)
        printf("Total time is %f \n", end - start);

    MPI_Finalize();
    return (0);
}
