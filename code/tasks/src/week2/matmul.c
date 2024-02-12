#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define it_limit 512

#define true 1
#define false 0

#define n_runs 1

int main(int argc, char *argv[]) {

    double **A;
    double **B;
    double **C;
    double *ptr1, *ptr2, *ptr3;
    int i, j, k, l;

    int dim; // dimension of matrix

    double mt1, mt2; // Timing variables
    float t_bs;

    printf("Give matrix dimension \n");
    scanf("%d", &dim);

    printf("Allocating memory \n");

    A = (double **)malloc(sizeof(double *) * dim);
    ptr1 = (double *)malloc(sizeof(double) * dim * dim);
    B = (double **)malloc(sizeof(double *) * dim);
    ptr2 = (double *)malloc(sizeof(double) * dim * dim);
    C = (double **)malloc(sizeof(double *) * dim);
    ptr3 = (double *)malloc(sizeof(double) * dim * dim);

    if (C == NULL) {
        printf("Unable to allocate memory, exiting \n");
        exit(0);
    }
    printf("Initializing \n");

    for (i = 0; i < dim; i++) {
        A[i] = ptr1 + (i * dim);
        B[i] = ptr2 + (i * dim);
        C[i] = ptr3 + (i * dim);
    }

    // ****************************
    // Starting the main algorithm
    // ****************************

    t_bs = -1.0;

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            A[i][j] = 1.0;
            B[i][j] = 1.0;
        }
    }

    printf("Starting the Matrix multiplication algorithm \n");

    for (l = 0; l < n_runs; l++) {
        mt1 = omp_get_wtime();

        //*** Main loop ***

        for (i = 0; i < dim; i++) {
            for (j = 0; j < dim; j++) {
                C[i][j] = 0.0;
                for (k = 0; k < dim; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                } // k
            }     // j
        }         // i

        mt2 = omp_get_wtime();

        //*** Capture best run

        if ((t_bs < 0) || (mt2 - mt1 < t_bs))
            t_bs = mt2 - mt1;
    } // l

    printf("Done computing \n");

    printf("Matrix multiplication with %d x %d matrices took %f seconds\n", dim, dim, t_bs);
    printf("%lf\n", C[0][0]);
    // flops = dim*dim*(2*dim-1);
}
