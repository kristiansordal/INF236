#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/* This code shows how different access patterns effect
 * the running time of a program.
 * Compile and run the program using the following options:
 * No options, -g, -O1, -O2 , -O3
 *
 * Always use -fopenmp for the timing.
 *
 * Also, try to run the program without the final print
 * statement while using -O3. 
 */

#define iterations 9  // Uses 10^i elements in iteration i.

void linear(double *sum1,int size,double *data) {
      int i;
      for(i=0;i<size;i++) {
        *sum1 += data[i];
      }
}

void indirect(double *sum2,int size,double *data,int *access) {
      int i;
      for(i=0;i<size;i++) {
        *sum2 += data[access[i]];
      }
}

void rand_pat(double *sum3,int size,double *data,int *access) {
      int i;
      for(i=0;i<size;i++) {
        *sum3 += data[access[i]];
      }
}

int main(int argc, char *argv[]) {

    int i,j;

    double *data;                 // Storage for data elements
    int *access;                  // Access pattern for elements in data

    int size = 1;                 // Length of array being accessed
    double global_sum = 0.0;      // Sum of elements in array

    for(j=0;j<iterations;j++) {
// Allocate memory for the data elements
      data = (double *) malloc(size*sizeof(double));

// Store random values in the data array
      for(i=0;i<size;i++) {
        data[i] = drand48();    // Fill the data array with random doubles
      }
 
// Sum up the data elements in a linear fashion
      double start = omp_get_wtime();   // Timer start
      double sum1 = 0.0;
        linear(&sum1,size,data);
      double end = omp_get_wtime();     // Timer ends
      double time1 = end - start;

// Compute a linear access pattern 
      access = (int *) malloc(size*sizeof(int));  // Allocate space for the pattern
      for(i=0;i<size;i++) {
        access[i] = i;          // Define the access pattern
      }

// Sum up the data elements in a linear fashion but now with indirection
      start = omp_get_wtime();
      double sum2 = 0.0;
        indirect(&sum2,size,data,access);
      end = omp_get_wtime();
      double time2 = end - start;

// Compute a random access pattern 
      for(i=0;i<size;i++) {
        int r = random() % (size-i);
        int temp = access[i];
        access[i] = access[i+r];
        access[i+r] = temp;
      }

// Sum up the data elements in random order
      start = omp_get_wtime();
      double sum3 = 0.0;
        rand_pat(&sum2,size,data,access);
      end = omp_get_wtime();
      double time3 = end - start;

      global_sum += sum1 + sum2 + sum3;

// Print statistics
      printf("n = %10d \t",size);
      printf("Timing: %lf, %lf, %lf \t",time1,time2,time3);
      printf("Ratio: %1.1lf, %1.2lf, %1.2lf \n",time1/time1,time2/time1,time3/time1);

      size = size*10;
    }

// What happens if we remove this line?
    printf("\nGlobal sum: %1.0lf\n",global_sum);

}
