#include <stdio.h>
#include <mpi.h>

int main (argc, argv)
int argc;
char *argv[];
{
  int rank, size;
  MPI_Init (&argc, &argv); 				/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank); 	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size); 	/* get number of processes */
  printf( "Hello world from process %d of %d\n", rank, size );

  int i,svar;
  svar = 0;
  #pragma omp parallel for reduction(+:svar)
  for(i=0;i<10000;i++)
    svar += i;

  printf( "Process %d has answer %d\n", rank, svar);
  MPI_Finalize();
  return 0;
}
