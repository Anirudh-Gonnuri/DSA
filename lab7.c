#include <stdio.h>
#include <stdlib.h>

#define INF 999999

int main() {
    int V, E, directed;
    
    printf("FITNESS TRACKER NETWORK - FLOYD WARSHALL IMPLEMENTATION\n");

    printf("Enter number of nodes (e.g., users/gyms/trainers): ");
    scanf("%d", &V);

    if (V <= 0 || V > 50) {
        printf("Invalid number of vertices.\n");
        return 1;
    }

    int dist[V][V];

    // Initialize adjacency matrix
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }

    printf("Enter number of connections/edges (e.g., time/distance between fitness nodes): ");
    scanf("%d", &E);

    if (E < 0 || E > V*(V-1)) {
        printf("Invalid number of edges.\n");
        return 1;
    }

    printf("Is the graph directed? (1 = Yes, 0 = No): ");
    scanf("%d", &directed);

    printf("\n--- Enter Edges (Format: source destination cost) ---\n");
    for (int i = 0; i < E; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);

        if (u < 0 || u >= V || v < 0 || v >= V || w < 0) {
            printf("Invalid edge input.\n");
            return 1;
        }

        dist[u][v] = w;
        if (!directed) dist[v][u] = w;
    }

    printf("\nInitial Distance Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++)
            printf("%7d ", dist[i][j] == INF ? -1 : dist[i][j]);
        printf("\n");
    }

    // Floyd Warshall Algorithm
    for (int k = 0; k < V; k++) {

        printf("\nUsing node %d as an intermediate:\n", k);

        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {

                // Optimization: skip if no useful path exists
                if (dist[i][k] == INF || dist[k][j] == INF)
                    continue;

                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }

        // Print intermediate matrix
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++)
                printf("%7d ", dist[i][j] == INF ? -1 : dist[i][j]);
            printf("\n");
        }
    }

    printf("\nFinal All-Pairs Shortest Path Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++)
            printf("%7d ", dist[i][j] == INF ? -1 : dist[i][j]);
        printf("\n");
    }

    return 0;
}
