// Parallel Breadth First Search
// -----------------------------
// Berforms a BFS starting from vertex 1
// The parent of each vertex in the BFS tree along with its distance from the starting
// vertex is computed.
//
// The algorithm should first perform some rounds of sequential BFS before starting a parallel
// execution. In the parallel part each thread should be allocated a part of the vertices from the
// last round of the sequential algorithm. Any discovered vertices in the parallel part should
// remain with the thread that discovered them. This continues until the entire graph has been
// explored.
//
//
// Parameters:
// n     : number of vertices
// ver   : ver[i] points to the start of the neighbor list of vertex i in edges
// edges : lists of neighbors of each vertex, each edge is listed in both direction
// p     : array of length n used for parent pointers
// dist  : array of length n used for distance from starting vertex
// S     : array of length n used for maintaining queue of vertices to be processed, only used in the
//         sequential part.
// T     : array of length n where n >> number of threads.
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.
//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sequential_steps(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int layer_size, num_discovered, *temp, flips = 0;
    int *S_original = S;
    int *T_original = T;

    for (int i = 1; i <= n; i++) {
        p[i] = -1;
        dist[i] = -1;
    }

    p[1] = 1;
    dist[1] = 0;
    S[0] = 1;

    layer_size = 1;
    num_discovered = 0;

    while (layer_size <= omp_get_num_threads() && layer_size != 0) {
        for (int i = 0; i < layer_size; i++) {
            int v = S[i];
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];
                if (p[u] == -1) {
                    p[u] = v;
                    dist[u] = dist[v] + 1;
                    T[num_discovered++] = u;
                }
            }
        }
        temp = S;
        S = T;
        T = temp;
        layer_size = num_discovered;
        flips++;

        num_discovered = 0;
    }

    if (flips % 2 != 0)
        for (int i = 0; i < layer_size; i++)
            S_original[i] = T_original[i];

    return layer_size;
}

void abfs(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int layer_size = 1, num_discovered = 0, depth = 0;
    int tid = omp_get_thread_num(), threads = omp_get_num_threads();
    int *discovered, *temp;
    int *local_S, local_layer_size = 0;
    int k = 8, k_steps = 0;

    // Allocate memory for discovered vertices, private for each rank
    discovered = malloc(n * sizeof(int));
    local_S = malloc(n * sizeof(int));
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

    // Explore k layers sequentially
#pragma omp master
    { T[0] = sequential_steps(n, ver, edges, p, dist, S, T); }
#pragma omp barrier

    // populate local_S
    int chunk = T[0] / threads;
    int start = chunk * tid;
    int end = tid == threads - 1 ? T[0] : chunk * (tid + 1);

    for (int i = start; i < end; i++)
        local_S[local_layer_size++] = S[i];

    while (layer_size != 0) {
#pragma omp barrier
        k_steps = depth % k == 0 && depth != 0;
        for (int i = 0; i < local_layer_size; i++) {
            int v = local_S[i];
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                int u = edges[j];
#pragma omp critical
                {
                    if (p[u] == -1) {
                        p[u] = v;
                        dist[u] = dist[v] + 1;
                        discovered[num_discovered++] = u;
                    }
                }
#pragma omp critical
                {
                    if (dist[u] > dist[v] + 1 && dist[v] != -1 && p[u] != -1) {
                        p[u] = v;
                        dist[u] = dist[v] + 1;
                        discovered[num_discovered++] = u;
                    }
                }
            }
        }

        depth++;
        T[tid] = num_discovered;

#pragma omp barrier // Syncronize, threads might not do any work, or finish before others
        layer_size = T[0];
        int offset = 0;
        for (int i = 1; i < threads; i++) {
            if (i == tid)
                offset = layer_size;
            layer_size += T[i];
        }

        if (k_steps) {
            memcpy(S + offset, discovered, num_discovered * sizeof(int));
            int chunk = layer_size / threads;
            int start = chunk * tid;
            int end = tid == threads - 1 ? layer_size : chunk * (tid + 1);
            local_layer_size = end - start;
            memcpy(local_S, S + start, local_layer_size * sizeof(int));

        } else {
            temp = local_S;
            local_S = discovered;
            discovered = temp;
            local_layer_size = num_discovered;
        }
        num_discovered = 0;
    }
}

// Write code here
