
// Sequential Graph Coloring
// -------------------------------
// Compute a greedy graph coloring 
//
// Parameters:
// n     : number of vertices
// ver   : ver[v] points to the start of the neighbor list of vertex v in edges
// edges : lists of neighbors of each vertex, each edge is listed in both direction
// col   : array of length n used for storing the color of each vertex 
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.

void sgc(int n,int *ver,int *edges,int *col) {

  int v,w;              // Pointers to vertices
  int j;                // Loop index

  for(v=1;v<=n;v++) {   // Set that every node is initially uncolored
    col[v] = 0; 
  }
  int maxColor = 200;   // Hope that we will not need more than 200 colors..
  int used[maxColor];   // If used[i] = v then v has a neighbor with color i

  for(v=0;v<maxColor;v++) {  // Set that no color is used initially
    used[v] = 0;
  }

  for(v=1;v<=n;v++) {                // Loop over vertices in G
    for(j=ver[v];j<ver[v+1];j++) {   // Go through the neighbors of v
      w = edges[j];                  // Get next neighbor w of v
      used[col[w]] = v;              // Mark all colors used by neighbors of v
                                     // Note that if w is uncolored then col[w] = 0
    }                                // End loop over neighbors of v

    j=1;                             // Find first available color (>0) for v
    while ((j<maxColor) && (used[j]==v))
      j++;                           // Try next color

    if (j==maxColor)
      printf("Not enough space for colors!!!\n");
    else
      col[v] = j;                    // Set the color of v to j
  }                                  // End loop over entire graph
}
