#include "../common/common.h"
#include <stdio.h>

/*
 * A simple introduction to programming in CUDA. This program prints "Hello
 * World from GPU! from 10 CUDA threads running on the GPU.
 */

__global__ void helloFromGPU() { printf("Hello World from GPU!, %d\n", threadIdx.x); }

int main(int argc, char **argv) {
    printf("Hello World from CPU!\n");

    helloFromGPU<<<1, 100>>>();
    CHECK(cudaDeviceReset());
    return 0;
}
