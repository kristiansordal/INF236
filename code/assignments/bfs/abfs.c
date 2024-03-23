#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sequential_steps(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int l, d, *temp, flips = 0;
    int *S_original = S;
    int *T_original = T;

    for (int i = 1; i <= n; i++) {
        p[i] = -1;
        dist[i] = -1;
    }

    p[1] = 1;
    dist[1] = 0;
    S[0] = 1;

    l = 1;
    d = 0;

    while (l <= omp_get_num_threads() && l != 0) {
        for (int i = 0; i < l; i++) {
            int u = S[i];
            for (int j = ver[u]; j < ver[u + 1]; j++) {
                int v = edges[j];
                if (p[v] == -1) {
                    p[v] = v;
                    dist[v] = dist[u] + 1;
                    T[d++] = v;
                }
            }
        }
        temp = S;
        S = T;
        T = temp;
        l = d;
        flips++;
        d = 0;
    }

    if (flips % 2 != 0)
        for (int i = 0; i < l; i++)
            S_original[i] = T_original[i];

    for (int i = 0; i < l; i++) {
        printf("To distribute: %d\n", S_original[i]);
    }

    return l;
}

void abfs(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int u, v, l, d, offset, l_tot = 0, k = 10;
    int *discovered, *queue;
    int *temp;
    int threads = omp_get_num_threads(), tid = omp_get_thread_num();

#pragma omp for
    for (int i = 1; i <= n; i++) {
        p[i] = -1;
        dist[i] = -1;
    }

    discovered = malloc(n * sizeof(int));
    queue = malloc(n * sizeof(int));

    p[1] = 1;
    dist[1] = 0;

    l = 0;
    d = 0;

    // Perform some rounds of sequential BFS
#pragma omp master
    { T[0] = sequential_steps(n, ver, edges, p, dist, S, T); }

    printf("t[0]; %d\n", T[0]);
#pragma omp barrier
#pragma omp for schedule(static)
    for (int i = 0; i < T[0]; i++)
        queue[l++] = S[i];

#pragma omp critical
    { printf("tid %d, %d\n", tid, l); }
    l_tot = T[0];

    while (l_tot != 0) {
#pragma omp barrier
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < l; j++) {
                u = queue[j];
                for (int w = ver[u]; w < ver[u + 1]; w++) {
                    v = edges[w];
                    if (p[v] == -1 || dist[v] > dist[u] + 1) {
                        p[v] = u;
                        dist[v] = dist[u] + 1;
                        discovered[d++] = v;
                    }
                }
            }
            temp = queue;
            queue = discovered;
            discovered = temp;
            l = d;
            d = 0;
        }
        T[tid] = l;
#pragma omp barrier
        l_tot = T[0];
        offset = 0;
        for (int i = 1; i < threads; i++) {
            if (i == tid) {
                offset = l_tot;
            }
            l_tot += T[i];
        }
        printf("tid: %d offset: %d, %d\n", tid, offset, l);
#pragma omp barrier
        // printf("tid: %d l_tot: %d, offset: %d\n", tid, l_tot, offset);
        memcpy(S + offset, discovered, l * sizeof(int));
        l = 0;

        int chunk = l_tot / threads;
        int start = tid * chunk;
        int end = (tid == threads - 1) ? l_tot : start + chunk;
        for (int i = start; i < end; i++) {
            queue[l++] = S[i];
            printf("tid: %d, q[%d]: %d\n", tid, l - 1, queue[l - 1]);
        }

        printf("thread %d will search %d vertices\n", tid, l);
    }
}
