#include <stdbool.h>
// Sequential Independent Set
// -------------------------------
// Compute a greedy IS
//
// Parameters:
// n     : number of vertices
// ver   : ver[v] points to the start of the neighbor list of vertex v in edges
// edges : lists of neighbors of each vertex, each edge is listed in both direction
// is    : array of length n used for marking vertices in the IS (true or false)
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.

void sis(int n, int *ver, int *edges, int *is) {

    int v, w; // Pointers to vertices
    int j;    // Loop index

    for (v = 1; v <= n; v++) { // Set that every node is initially out of the IS
        is[v] = false;
    }

    for (v = 1; v <= n; v++) {                  // Loop over vertices in G
        int inSet = true;                       // Assume vertex v is in the IS
        for (j = ver[v]; j < ver[v + 1]; j++) { // Go through the neighbors of v
            w = edges[j];                       // Get next neighbor w of v
            if (is[w]) {                        // Check if w is in the IS
                inSet = false;                  // If so, then v is not in the IS
                break;                          // No need to continue checking neighbors
            }
        }              // End loop over neighbors of v
        is[v] = inSet; // Store the status of vertex i
    }                  // End loop over entire graph
}
