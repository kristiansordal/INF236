#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sequential_steps(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int layer_size, num_discovered, *temp, flips = 0;
    int *S_original = S;
    int *T_original = T;
    printf("S: %p", S);
    printf("T: %p", T);

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
                    printf("discovered %d\n", u);
                    T[num_discovered++] = u;
                }
            }
        }
        temp = S;
        S = T;
        T = temp;
        printf("S: %p", S);
        printf("T: %p", T);
        layer_size = num_discovered;
        flips++;
        // if now flips is odd - then S points to T
        num_discovered = 0;
    }

    // if (flips % 2 != 0) {
    //     // Copy contents of T to S_original if flips is even
    //     for (int i = 0; i < layer_size; i++) {
    //         S_original[i] = T[i];
    //     }
    // }

    return layer_size;
}

void abfs(int n, int *ver, int *edges, int *p, int *dist, int *S, int *T) {
    int u, v, l, d, l_tot = 0, k = 4;
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
    S[0] = 1;

    l = 0;
    d = 0;

    // Perform some rounds of sequential BFS
#pragma omp single
    {
        T[0] = sequential_steps(n, ver, edges, p, dist, S, T);
        for (int i = 0; i < T[0]; i++) {
            printf("%d %d\n", T[i], S[i]);
        }
    }

#pragma omp for schedule(static)
    for (int i = 0; i < T[0]; i++)
        queue[l++] = S[i];

    for (int i = 0; i < l; i++) {
        printf("queue[%d]: %d\n", i, queue[i]);
    }

    while (l_tot != 0) {
#pragma omp barrier
        for (int i = 0; i < k; i++) {
            d = 0;
            for (int i = 0; i < l; i++) {
                u = S[i];
                for (int j = ver[u]; j < ver[u + 1]; j++) {
                    v = edges[j];
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
        }
#pragma omp barrier
        T[tid] = d;

        l_tot = T[0];
        int offset = 0;
        for (int i = 1; i < threads; i++) {
            if (i == tid)
                offset = l_tot;
            l_tot += T[i];
        }

        memcpy(S + offset, discovered, d * sizeof(int));
        l = 0;
#pragma omp for schedule(static)
        for (int i = 0; i < d; i++)
            queue[l++] = discovered[i];
    }
}
