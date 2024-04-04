#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* This program shows how to perform matrix vector multiplication
 * Data is generated on processor 0 before being distributed.
 * The final result is again gathered on processor 0.
 */

#define max_pros 100    // Maximum number of process the program can handle

#define dim 5           // Dimension of matrix, assume only known to process 0

int main(argc,argv) 
int argc;
char **argv;
{

  int myrank;    // Rank of each process
  int np;        // Total number of processes

  int i,j;       // Loop indices
  
  double A[dim][dim];  // Global array, only used on process 0

  double *x;           // x vector
  double *res;         // local result vector
  double *globalRes;   // global result vector, only used on process 0

// Arrays used for sending out A, only significant on process 0
  int start[max_pros];
  int numRows[max_pros];
  int numElements[max_pros];

  int globalDim;  // The dimension of A, used on every process
  int localDim;   // The number of rows each process gets, used on every process

// Start MPI

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

//  printf("Hello from process %d out of %d \n",myrank,np);


// Broadcast the dimension of the matrix

  if (myrank == 0) {  // Assuming only process 0 knows this
    globalDim = dim;
    if (globalDim < np) {
      printf("Matrix dimension is smaller than number of processes, expect failure...\n");
    }
  }

// Broadcast the dimension of the matrix
  MPI_Bcast(&globalDim,1,MPI_INT,0,MPI_COMM_WORLD);

//  printf("%d got size=%d \n",myrank,globalDim);

// Allocate space for the x vector
  x = (double *) malloc(globalDim * sizeof(double));


// Initialize A and x
  if (myrank == 0) {

    srand48(1);
    // printf("Initial matrix: \n");
    for(i=0;i<globalDim;i++) {
      x[i] = drand48();
      for(j=0;j<globalDim;j++) {
        A[i][j] = drand48();
       // printf("%lf ",A[i][j]);
      }
      //printf("\n");
    }

// Now calculate how many rows and elements from A each process will receive

    int chunk,rest;
    chunk = globalDim / np; // Minimum number of rows given to each process.
    rest = globalDim % np;  // Remaining rows, must also be given to some process

    start[0] = 0;        // start[i] contains the position in A from which process i will start to receive
    for(i=0;i<np;i++) {
      numRows[i] = chunk;  // Every process gets at least chunk rows

      if (i < rest)        // The first "rest" processes gets an extra row
        numRows[i]++;

      numElements[i] = numRows[i]*globalDim;  // Calculate the number of elements from A each process gets

      if (i<np-1)
        start[i+1] = start[i] + numElements[i];  // Calculate from where process i+1 will start to receive
    }

    for(i=0;i<np;i++) {
   //   printf("%d starting at %d \n",i,start[i]);
    }   

// Do the entire computation on process 0 and print out the result, just for comparison
    double temp;
    for(i=0;i<globalDim;i++) {
      temp = 0.0;
      for(j=0;j<globalDim;j++) {
        temp = temp + A[i][j]*x[j];
      }
      printf("%lf \n",temp);
    }

  }

// Now send the x vector to everyone
  MPI_Bcast(x,globalDim,MPI_DOUBLE,0,MPI_COMM_WORLD);

// Next, tell each process the number of rows it will get from A
  MPI_Scatter(numRows,1,MPI_INT,&localDim,1,MPI_INT,0,MPI_COMM_WORLD);
  // printf("%d got local numRows =%d \n",myrank,localDim);

// Allocate space for the local result vector
  res = (double *) malloc(localDim * sizeof(double));

// Allocate space for the local part of A
  double *localSpace;
  localSpace = (double *) malloc(localDim*globalDim*sizeof(double)); 

  double **localA;
  localA = (double **) malloc(localDim*sizeof(double *));

  for(i=0;i<localDim;i++)
    localA[i] = &(localSpace[i*globalDim]);
// Done setting up local space


// Scatter A to the processes

  MPI_Scatterv(A,numElements,start,MPI_DOUBLE,localSpace,localDim*globalDim,MPI_DOUBLE,0,MPI_COMM_WORLD);

/*
  if (myrank == 1) {
    // printf("Process 2 received: \n");
    for(i=0;i<localDim;i++) {
      for(j=0;j<globalDim;j++) {
       // printf("%lf ",localA[i][j]);
      }
     // printf("\n");
    }
  }
*/
// Now do the local computation
  for(i=0;i<localDim;i++) {
    res[i] = 0.0;
    for(j=0;j<globalDim;j++) {
      res[i] = res[i] + localA[i][j]*x[j];
    }
  }

// Gather the result on process 0
  if (myrank == 0) {
// Allocate space for the global solution
    globalRes = (double *) malloc(globalDim*sizeof(double));

// Need displacements into global solution array
    numElements[0] = 0;
    for(i=1;i<np;i++)
      numElements[i] = numElements[i-1] + numRows[i-1];
  }

  MPI_Gatherv(res,localDim,MPI_DOUBLE,globalRes,numRows,numElements,MPI_DOUBLE,0,MPI_COMM_WORLD);

  if (myrank == 0) {
    printf("Final result\n");
    for(i=0;i<globalDim;i++) {
      printf("%lf \n",globalRes[i]);
    }
  }

  MPI_Finalize();
  return(0);
}
