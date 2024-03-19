
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

#include "util.c"
#include <omp.h>
#include <stdlib.h>
#include <string.h>

void pbfs(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int layer_size = 1, num_discovered = 0;
    int tid = omp_get_thread_num(), threads = omp_get_num_threads();
    int *discovered;
    omp_set_num_threads(1);

    // Allocate memory for discovered vertices, private for each rank
    discovered = malloc(n * sizeof(int));
    memset(discovered, 0, n * sizeof(int));

// Initialize shared variables
#pragma omp for
    for (int i = 0; i <= n; i++) {
        p[i] = -1;
        dist[i] = -1;
    }

    p[1] = 1;
    dist[1] = 0;
    S[0] = 1;

    while (layer_size != 0) {
#pragma omp barrier
        // Discover the layer in parallel
#pragma omp for nowait
        for (int i = 0; i < layer_size; i++) {
            int v = S[i];
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];
                discover(v, u, &num_discovered, p, dist, discovered);
            }
        }
        // Thread stores the number of discovered vertices
        T[tid] = num_discovered;

#pragma omp barrier // Syncronize, threads might not do any work, or finish before others
        layer_size = T[0];
        int offset = 0;
        for (int i = 1; i < threads; i++) {
            if (i == tid)
                offset = layer_size;
            layer_size += T[i];
        }

        if (num_discovered > 0) {
            memcpy(S + offset, discovered, num_discovered * sizeof(int));
            memset(discovered, 0, num_discovered * sizeof(int));
            num_discovered = 0;
        }
    }
}
