
// Parallel Independent set
// -----------------------------
// Compute an independent set using a parallel algorithm.
// The algorithm should first use a parallel loop over the vertices similar to the
// sequential IS algorithm. Following this each thread looks through its vertices
// to check if there are any conflicts, i.e. if two adjacent vertices are in the IS.
// In this case the lower numbered vertex is removed from the IS.
//
// It is also possible to collect the conflicting vertices in a shared array and let the
// master thread go through these sequentially at the end of the algorithm. This might lead
// to a larger solution. To do this you might need to use the shared arrays t1 and t2.
// Check the comments in the file pgc.c for how this can be done.
//
// Parameters:
// n     : number of vertices
// ver   : array of length n. ver[i] points to the start of the neighbor list of vertex i in edges
// edges : array containing lists of neighbors for each vertex, each edge is listed in both direction
// is    : shared array of length n used to store if a vertex is in the IS (true) or out (false)
// t1    : shared array of length n
// t2    : shared array of length n
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.

#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
void pis(int n, int *ver, int *edges, int *is, int *t1, int *t2) {
    int p = omp_get_num_threads();
    int conflicts[p];
    for (int i = 0; i < p; i++) {
        conflicts[i] = 0;
    }

#pragma omp for
    for (int i = 0; i < n; i++)
        is[i] = false;

#pragma omp for
    for (int v = 0; v <= n; v++) {
        bool in_set = true;
        for (int j = ver[v]; j < ver[v + 1]; j++) {
            if (is[edges[j]]) {
                in_set = false;
                break;
            }
        }
        is[v] = in_set;
    }

#pragma omp for
    for (int v = 0; v <= n; v++) {
        if (is[v]) {
            for (int j = ver[v]; j < ver[v + 1]; j++) {
                if (is[edges[j]] && edges[j] > v) {
                    is[v] = false;
                    conflicts[omp_get_thread_num()]++;
                    break;
                }
            }
        }
    }

    int total_conflicts = 0;
#pragma omp master
    {
        for (int i = 0; i < p; i++) {
            total_conflicts += conflicts[i];
        }
        printf("Number of conflicts: %d\n", total_conflicts);
    }
}
