
// Parallel Graph Coloring
// -----------------------------
// Performs a parallel greedy Graph Coloring 
//
// The algorithm should first use a parallel loop to greedily color each vertex. This
// is done similarly to the sequential algorithm. Then each thread should check the color
// of each of its vertices. If two adjacent vertices have the same color then the lower
// numbered vertex should be saved for recoloring. Next, all vertices that need to be 
// recolored should be stored consecutively in a shared array (hint, prefix sum). Finally,
// one thread will sequentialy recolor all vertices that need to be recolored.
//
// Parameters:
// n     : number of vertices
// ver   : array of length n. ver[i] points to the start of the neighbor list of vertex i in edges
// edges : array containing lists of neighbors for each vertex, each edge is listed in both direction
// col   : shared array of length n used for storing color values
// S     : shared array of length n used for prefix sum
// T     : shared array of length n used for storing vertices that should be recolored.
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.

void pgc(int n,int *ver,int *edges,int *col,int *S,int *T) {

// Write code here

}
