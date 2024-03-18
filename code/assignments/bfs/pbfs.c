
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
    int layer_size = 1, threads = omp_get_num_threads(), num_discovered = 0, *discovered;
    int tid = omp_get_thread_num();

    discovered = malloc(n * sizeof(int));
    memset(discovered, 0, n * sizeof(int));

#pragma omp single
    {
        memset(p, -1, (n + 1) * sizeof(int));
        memset(dist, -1, (n + 1) * sizeof(int));
        p[1] = 1;
        dist[1] = 0;
        S[0] = 1;
    }

    printf("Starting Search\n");
    while (layer_size != 0) {
#pragma omp barrier
#pragma omp for
        for (int i = 0; i < layer_size; i++) {
            int v = S[i];
            printf("Thread %d processing vertex %d\n", tid, v);
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];

                if (p[u] == -1) {
                    p[u] = v;
                    dist[u] = dist[v] + 1;
                    discovered[num_discovered++] = u;
                }
            }
        }
        T[tid] = num_discovered;
        printf("Thread %d discovered %d vertices\n", tid, T[tid]);
#pragma omp single
        {
            int s = T[0];
            layer_size = s;
            T[0] = 0;
            for (int i = 1; i < threads; i++) {
                layer_size += T[i];
                const int t = s + T[i];
                T[i] = s;
                s = t;
            }
        }

        if (num_discovered > 0) {
            memcpy(S + T[tid], discovered, num_discovered * sizeof(int));
            num_discovered = 0;
        }

#pragma omp single
        {
            for (int i = 0; i < layer_size; i++)
                T[i] = 0;
        }
    }
    printf("%d, done\n", tid);
}
