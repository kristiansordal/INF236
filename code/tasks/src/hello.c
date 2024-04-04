#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num;
    int *nums;
    if (rank == 0) {
        nums = malloc(size * sizeof(int));

        for (int i = 0; i < size; i++)
            nums[i] = i;
    }

    MPI_Scatter(nums, 1, MPI_INT, &num, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Rank %d got %d\n", rank, num);
    num *= 2;
    int sum = 0;

    if (rank == 0) {
        int recvbuf = 0;
        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(&recvbuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += recvbuf;
        }
    } else
        MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Rank %d sum %d\n", rank, sum);
    }

    MPI_Finalize();
    return 0;
}
