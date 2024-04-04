#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(argc,argv) 
int argc;
char **argv;
{

  int myrank;
  int np;

  MPI_Status status;
  int i,j,k,it;
  int size;
  double start,end;

  double *x;


  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  

// Main loop
  x = (double *) malloc(sizeof(double)*10000000);

  for(it=0;it<1000;it++) {
    size = it*1000;
    start = MPI_Wtime();

      if (myrank == 0)
        MPI_Send(x,size,MPI_DOUBLE,1,10,MPI_COMM_WORLD);
      else
        MPI_Recv(x,size,MPI_DOUBLE,0,10,MPI_COMM_WORLD,&status);

    end = MPI_Wtime();
    if (myrank == 0) {
      printf("Time is %f \n",end-start);
    }
  }

  MPI_Finalize();
  return(0);
}
