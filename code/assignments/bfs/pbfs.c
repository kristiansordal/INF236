
// Parallel Breadth First Search
// -----------------------------
// Performs a BFS starting from vertex 1
// The parent of each vertex in the BFS tree along with its distance from the starting
// vertex is computed.
//
// The algorithm should gather all discovered vertices from round i, so that they can be
// distributed among the threads before the search in round i+1.
//
// Parameters:
// n     : number of vertices
// ver   : array of length n. ver[i] points to the start of the neighbor list of vertex i in edges
// edges : array containing lists of neighbors for each vertex, each edge is listed in both direction
// p     : array of length n used for parent pointers
// dist  : array of length n used for distance from starting vertex
// S     : array of length n used for maintaining queue of vertices to be processed
// T     : array of length n where n >> number of threads.
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void pbfs(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int layer_size = 1, threads = omp_get_num_threads(), *num_discovered, *displs, **discovered;
    int tid = omp_get_thread_num();

#pragma omp master
    {
        printf("TID: %d\n", tid);
        discovered = malloc(threads * sizeof(int *));
        for (int i = 0; i < threads; i++) {
            discovered[i] = malloc(n * sizeof(int));
        }

        num_discovered = malloc(threads * sizeof(int));
        displs = malloc(threads * sizeof(int));

        for (int i = 1; i <= n; i++) {
            p[i] = -1;
            dist[i] = -1;
        }

        p[1] = 1;
        dist[1] = 0;
        S[0] = 1;
    }

    displs[tid] = 0;
    num_discovered[tid] = 0;
    for (int i = 0; i < n; i++)
        discovered[tid][i] = 0;

    printf("Starting Search\n");
#pragma omp barrier
    while (layer_size != 0) {
#pragma omp for nowait
        for (int i = 0; i < layer_size; i++) {
            int v = S[i];
            printf("Thread %d: Processing vertex %d\n", tid, v);
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];

                printf("Thread %d: Processing edge %d\n", tid, u);
                if (p[u] == -1) {
                    p[u] = v;
                    dist[u] = dist[v] + 1;
                    discovered[tid][num_discovered[tid]++] = u;
                }
                printf("Thread %d: Done processing edge %d\n", tid, u);
            }
        }
#pragma omp master
        {
            displs[0] = 0;
            layer_size = 0;
            for (int i = 0; i < threads; i++)
                printf("displs[%d],%d\n", i, displs[i]);

            for (int i = 1; i <= threads; i++) {
                layer_size += num_discovered[i - 1];
                displs[i] = displs[i - 1] + num_discovered[i - 1];
                printf("displs[%d] = %d\n", i, displs[i]);
            }
            layer_size = displs[threads - 1] + num_discovered[threads - 1]; // Total new vertices discovered
            printf("layer_size = %d\n", layer_size);
        }

        if (num_discovered[tid] > 0) {
            memcpy(S + displs[tid], discovered[tid], num_discovered[tid] * sizeof(int));
        }
        num_discovered[tid] = 0;
        memset(discovered[tid], 0, n * sizeof(int));
    }

    free(discovered);
    free(num_discovered);
    free(displs);
}
