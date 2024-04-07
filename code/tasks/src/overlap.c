#include <mpi.h>
#include <stdio.h>

#define dim 100

int main(argc, argv)
int argc;
char **argv;
{

    int myrank;
    int np;

    int i, j, k, it;

    double start, end;

    double A[dim][dim];
    double B[dim][dim];
    double C[dim][dim];
    double D[dim][dim];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    // Set up data localy on each process
    srand48(myrank);
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            A[i][j] = drand48();
            B[i][j] = drand48();
            C[i][j] = 0.0;
        }
    }

    // Main loop

    start = MPI_Wtime();
    for (it = 0; it < np; it++) {
        printf("it = %d \n", it);

        // Matrix multiplication
        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
                for (k = 0; k < dim; k++)
                    C[i][j] += A[i][k] * B[k][j];

        MPI_Request request;
        // Send B to the next process, receive in D
        printf("Sending \n");
        MPI_Isend(B, dim * dim, MPI_DOUBLE, (myrank + 1) % np, 10, MPI_COMM_WORLD, &request);
        printf("Receiving \n");
        MPI_Irecv(D, dim * dim, MPI_DOUBLE, (myrank - 1 + np) % np, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Copying \n");
        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
                B[i][j] = D[i][k];
    }
    end = MPI_Wtime();

    if (myrank == 0) {
        printf("Total time is %f \n", end - start);
    }

    MPI_Finalize();
    return (0);
}
