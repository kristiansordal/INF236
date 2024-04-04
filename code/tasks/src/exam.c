#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void MM(float **myA,float **myB,float **myCT,int a,int b,int c,int myRank) {
  int i,j,k;

  for(i=0;i<a;i++)
    for(j=0;j<c;j++) {
      myCT[i][j] = 0.0;
      for(k=0;k<b;k++) {
        myCT[i][j] += myA[i][k]*myB[k][j];
      }
    }
}

void moveMatrix(float **myCT,float **myC,int block,int dim) {
  int i,j;

  for(i=0;i<dim;i++)
    for(j=0;j<dim;j++)
      myC[i][j+dim*block] = myCT[i][j];
}

int main(argc,argv) 
int argc;
char **argv;
{

  int myRank;
  int n,p,i,j,k;
  MPI_Status status;

  MPI_Init(&argc,&argv);                       // Start MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);      // Get my id
  MPI_Comm_size(MPI_COMM_WORLD, &p);           // Get number of processes

  n = 10;
  if (n%p != 0) {
    printf("p must divide n\n");
    return(1);
  }

  float **myCT;
  float **myA;
  float **myC;
  float **myB;

  myCT = (float **) malloc(sizeof(float*)*n/p);
  myA =  (float **) malloc(sizeof(float*)*n/p);
  myB =  (float **) malloc(sizeof(float*)*n);
  myC =  (float **) malloc(sizeof(float*)*n/p);

  float *p1,*p2,*p3,*p4;
  p1 = (float *) malloc(sizeof(float)*n/p*n/p);
  p2 = (float *) malloc(sizeof(float)*n*n/p);
  p3 = (float *) malloc(sizeof(float)*n*n/p);
  p4 = (float *) malloc(sizeof(float)*n*n/p);

  for(i=0;i<n/p;i++) {
    myCT[i] = p1 + i*(n/p);
    myA[i]  = p2 + i*n;
    myC[i]  = p3 + i*n;
  }
  for(i=0;i<n;i++) 
    myB[i]  = p4 + i*(n/p);

  for(i=0;i<n*n/p;i++) { 
    p2[i] = 1.0;
    p4[i] = 2.0;
  }

  MM(myA,myB,myCT,n/p,n,n/p,myRank);           // First multiplication of initial values
  moveMatrix(myCT,myC,myRank,n/p);             // Move the result into myC

  for(i=1;i<p;i++) {
    MPI_Send(&(myB[0][0]),n*n/p,MPI_FLOAT,(myRank+1)%p,1, MPI_COMM_WORLD);    // send&receive myB
    MPI_Recv(&(myB[0][0]),n*n/p,MPI_FLOAT,(myRank-1+p)%p,1,MPI_COMM_WORLD,&status); 
    MM(myA,myB,myCT,n/p,n,n/p,myRank);
    moveMatrix(myCT,myC,(myRank+i)%p,n/p);
  }

  printf("%d: Final %4.1f %4.1f %4.1f \n",myRank,myC[0][0],myC[0][1],myC[0][2]);
  MPI_Finalize();
  return(0);
}


