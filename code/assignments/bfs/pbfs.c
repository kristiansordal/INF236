
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
    int layer_size = 1, *num_discovered, **discovered;
    int tid = omp_get_thread_num();

#pragma omp single
    {
        discovered = malloc(omp_get_num_threads() * sizeof(int *));
        num_discovered = malloc(omp_get_num_threads() + 1 * sizeof(int));
        for (int i = 0; i < omp_get_num_threads(); i++)
            discovered[i] = malloc(n * sizeof(int));

        memset(p, -1, n * sizeof(int));
        memset(dist, -1, n * sizeof(int));
        memset(num_discovered, 0, omp_get_num_threads() + 1);

        p[1] = 1;
        dist[1] = 0;
        S[0] = 1;
    }

    printf("Starting Search\n");

    while (layer_size != 0) {
#pragma omp for
        for (int i = 0; i < layer_size; i++) {
            int v = S[i];
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];

                if (p[u] == -1) {
                    p[u] = v;
                    printf("parent of %d is %d\n", u, v);
                    dist[u] = dist[v] + 1;
                    discovered[tid][num_discovered[tid]++] = u;
                }
            }
        }

#pragma omp master
        {
            int s = num_discovered[0];
            num_discovered[0] = 0;
            for (int i = 1; i < omp_get_num_threads() + 1; i++) {
                const int t = s + num_discovered[i];
                num_discovered[i] = s;
                s = t;
            }
            layer_size = num_discovered[omp_get_num_threads()];
        }

        for (int i = num_discovered[tid]; i < num_discovered[tid + 1]; i++) {
            S[i] = discovered[tid][i - num_discovered[tid]];
        }

        num_discovered[tid] = 0;
    }

    free(discovered);
    free(num_discovered);
}
