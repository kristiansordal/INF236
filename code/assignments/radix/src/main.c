#if defined(PAR) || defined(COMPARE)
#include "radix_par.h"
#endif
#if defined(SEQ) || defined(COMPARE)
#include "radix_seq.h"
#endif
#include <stdio.h>
int main() {
    int size, bits;
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    printf("Enter the key size: ");
    scanf("%d", &bits);
    printf("Elements: %d, key size: %d\n", size, bits);

#if defined(SEQ) || defined(COMPARE)
    double seq_time = radix_sort_seq(size, bits);
    printf("SEQUENTIAL time: %f\n", seq_time);
#endif
#if defined(PAR) || defined(COMPARE)
    double par_time = radix_sort_par(size, bits);
    printf("PARALLEL time: %f\n", par_time);
#endif

#ifdef COMPARE
    double speedup_ratio = seq_time / par_time;
    double speedup_percentage = (speedup_ratio - 1.0) * 100.0;

    printf("Speedup ratio: %.3f\n", speedup_ratio);
    printf("Speedup prcnt: %.3f%%\n", speedup_percentage);
#endif

    return 0;
}
