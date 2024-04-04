#include <stdio.h>
#include <mpi.h>

/* Program showing communication from processor 0 to every other processor
 */

int main(argc,argv) 
int argc;
char **argv;
{

  int myrank;
  int np;
  int x;
  MPI_Status status;
  int i;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  
  printf("I am %d out of %d processors \n",myrank,np);

  if (myrank == 0) {
    x = 11;
    for(i=1;i<np;i++) {
      MPI_Send(&x,1,MPI_INT,i,1,MPI_COMM_WORLD);
    }
  }
  else {
    MPI_Recv(&x,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
    printf("Processor %d got %d \n",myrank,x);
  }

  MPI_Finalize();
  return(0);
}
