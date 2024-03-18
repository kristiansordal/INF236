
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
    int num_r = 1, num_u = 0, *temp, local_u = 0;
    int *T_local = malloc(n * sizeof(int));

#pragma omp single
    {
        memset(p, -1, n * sizeof(int));
        memset(dist, -1, n * sizeof(int));
        p[1] = 1;
        dist[1] = 0;
        S[0] = 1;
    }

#pragma omp barrier

    while (num_r != 0) {
#pragma omp for
        for (int i = 0; i < num_r; i++) {
            int v = S[i];
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];
                if (p[u] == -1) {
                    p[u] = v;
                    dist[u] = dist[v] + 1;
                    T_local[local_u++] = u;
                }
            }
        }

#pragma omp critical
        {
            for (int i = 0; i < local_u; i++) {
                T[num_u++] = T_local[i];
                printf("T[%d]: %d", num_u - 1, T[num_u - 1]);
                T_local[i] = 0;
            }
            local_u = 0;
        }

#pragma omp master
        {
            temp = S; // Swap S and T
            S = T;
            T = temp;

            num_r = num_u;
            num_u = 0;
        }
    }

    free(T_local);
}
