#if defined(PAR) || defined(COMPARE)
#include "radix_par.h"
#endif
#if defined(SEQ) || defined(COMPARE)
#include "radix_seq.h"
#endif
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <size> <bits>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int size = atoi(argv[1]), bits = atoi(argv[2]);
    double par_time, seq_time;

#ifdef SEQ
    int keys[5] = {1, 2, 4, 8, 16};
    int elems[10] = {1e5, 1e6, 1e7, 1e8};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            seq_time = radix_sort_seq(elems[j], keys[i]);
            printf("SEQUENTIAL time: %f\n", seq_time);
        }
    }
#endif
#ifdef PAR
    par_time = radix_sort_par(size, bits);
    printf("PARALLEL time: %f\n", par_time);
#endif

#ifdef COMPARE
    int keys[5] = {1, 2, 4, 8, 16};
    int elems[10] = {1e5, 1e6, 1e7, 1e8};

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            printf("===================================\n");
            printf("Array size      : %d\n", elems[j]);
            printf("Key size        : %d\n", keys[i]);
            seq_time = radix_sort_seq(elems[j], keys[i]);
            par_time = radix_sort_par(elems[j], keys[i]);
            double speedup_ratio = seq_time / par_time;
            double speedup_percentage = (speedup_ratio - 1.0) * 100.0;
            printf("Sequential time : %.3f sec\n", seq_time);
            printf("Parallel time   : %.3f sec\n", par_time);
            printf("Speedup prcnt   : %.3f%%\n", speedup_percentage);
        }
    }
#endif

    return 0;
}
