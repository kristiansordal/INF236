#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2) {
        printf("This program requires 2 proccesses\n");
        return 0;
    }

    int send_size;
    scanf("%d", &send_size);
    int *x = malloc(send_size * sizeof(int));
    memset(x, 0, send_size * sizeof(int));

    for (int i = 0; i < send_size; i++) {
        double time = 0;
        if (rank == 0) {
            time = MPI_Wtime();
            MPI_Send(x, i, MPI_INT, 1, 0, MPI_COMM_WORLD);
            time = MPI_Wtime() - time;
            printf("Sending %d elements took %f seconds\n", i, time);

        } else {
            sleep(1);
            MPI_Recv(x, i, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    MPI_Finalize();
}
