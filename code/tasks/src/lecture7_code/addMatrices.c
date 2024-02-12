#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define it_limit 512

#define true 1
#define false 0

#define n_runs 1

int main(int argc, char *argv[]) {

  int **a;    
  int *aptr;
  int **b;    
  int *bptr;
  int **c;    
  int *cptr;


  int dim;  // dimension of picture

  double mt1,mt2; // Timing variables
  float t_bs;

  printf("Give grid size\n");
  scanf("%d",&dim);

  printf("Allocating memory \n");

  a = malloc(sizeof(int *)*dim);
  aptr = malloc(sizeof(int)*dim*dim);
  b = malloc(sizeof(int *)*dim);
  bptr = malloc(sizeof(int)*dim*dim);
  c = malloc(sizeof(int *)*dim);
  cptr = malloc(sizeof(int)*dim*dim);

  printf("Initializing \n");
  int i,j;

  for(i=0;i<dim;i++) {    
      a[i] = aptr + (i*dim);
      b[i] = bptr + (i*dim);
      c[i] = cptr + (i*dim);
  }

  for(i=0;i<dim;i++) {
    for(j=0;j<dim;j++) {
      a[i][j] = 0;
      b[i][j] = 1;
      c[i][j] = 2;
    }
  }

  // ****************************
  // Starting the main algorithm
  // ****************************

  printf("Starting adding matrices\n");

  t_bs = -1.0;


  int l;
  for(l=0;l<n_runs;l++) {
    mt1 = omp_get_wtime();

//*** Main loop ***

#pragma omp parallel 
{
    int j;

#pragma omp for schedule(runtime) private(j)
    for(i=0;i<dim;i++) {
      for(j=0;j<dim;j++) {
        a[i][j] = b[i][j] + c[i][j];
      } // j
    } // i

}

    mt2 = omp_get_wtime();

//*** Capture best run

    if ((t_bs < 0) || (mt2-mt1 < t_bs))
      t_bs = mt2-mt1;
  }

  printf("Done computing \n");

//*** Print out small instances

  if (dim <= 50) {
    for(i=0;i<dim;i++) {
      for(j=0;j<dim;j++) {
        printf("%d ",a[i][j]);
      }
      printf("\n");
    }
  }

  printf("Adding two matrices of dimension %d x %d took %f seconds\n",dim,dim,t_bs);

}

