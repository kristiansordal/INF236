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

  int **pict;    
  int *ptr;
  int i,l;

  int dim;  // dimension of picture

  double mt1,mt2; // Timing variables
  float t_bs;
  int inMandel();

  printf("Give grid size\n");
  scanf("%d",&dim);

  printf("Allocating memory \n");

  pict = malloc(sizeof(int *)*dim);
  ptr = malloc(sizeof(int)*dim*dim);

  printf("Initializing \n");

  for(i=0;i<dim;i++) {    
      pict[i] = ptr + (i*dim);
  }

  // ****************************
  // Starting the main algorithm
  // ****************************

  printf("Starting Mandelbrot algorithm \n");

  t_bs = -1.0;

  double x,y;
  double x_min,x_max,y_min,y_max;

  x_min = -2.0;
  x_max = 2.0;
  y_min = -2.0;
  y_max = 2.0;

  int nr_x = dim;
  int nr_y = dim;
  
  double x_step = (x_max-x_min)/nr_x;
  double y_step = (y_max-y_min)/nr_y;

  for(l=0;l<n_runs;l++) {
    mt1 = omp_get_wtime();

//*** Main loop ***

#pragma omp parallel 
{
    double a,b;
    int j;

#pragma omp for schedule(runtime)
    for(i=0;i<dim;i++) {
      b = y_max - (i+0.5)*y_step;
      for(j=0;j<dim;j++) {
        a = x_min + (j+0.5)*x_step;
        if (inMandel(a,b))
          pict[i][j] = true;
        else
          pict[i][j] = false;
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

  int j;
  if (dim <= 50) {
    for(i=0;i<dim;i++) {
      for(j=0;j<dim;j++) {
        printf("%d ",pict[i][j]);
      }
      printf("\n");
    }
  }

  printf("Mandelbrot computation on %d x %d grid took %f seconds\n",dim,dim,t_bs);
  //flops = dim*dim*(2*dim-1);


}

int inMandel(double a, double b) {

  int it = 0;
  double x = 0.0;
  double y = 0.0;
  double temp;

  while ((it < it_limit)  && ((x*x + y*y) < (double)4.0)) {
    temp = x*x - y*y + a;
    y = 2*x*y + b;
    x = temp;
    it++;
  }
  if (it == it_limit)
    return(true);
  else
    return(false);

}
