void discover(int v, int u, int *num_discovered, int *p, int *dist, int *discovered) {
    if (p[u] == -1) {
        p[u] = v;
        dist[u] = dist[v] + 1;
        discovered[*num_discovered++] = u;
    }
}
