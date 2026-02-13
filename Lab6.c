#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 20

int graph[MAX][MAX], n;

// ---------------------------- VALIDATION ----------------------------
int validate_vertex(int v) {
    return (v >= 0 && v < n);
}

// ---------------------------- PRINT ADJACENCY MATRIX ----------------------------
void printAdjMatrix() {
    // Print column headings
    printf("\n     ");
    for (int i = 0; i < n; i++)
        printf("%3d ", i);
    printf("\n");

    // Print row headings + matrix
    for (int i = 0; i < n; i++) {
        printf("%3d  ", i);   // Row heading
        for (int j = 0; j < n; j++)
            printf("%3d ", graph[i][j]);
        printf("\n");
    }
}


// ---------------------------- DIJKSTRA ----------------------------
void dijkstra(int src) {
    int dist[MAX], visited[MAX];

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[src] = 0;

    printf("\nDijkstra Trace (src=%d):\n", src);

    for (int count = 0; count < n - 1; count++) {
        int u = -1;

        for (int i = 0; i < n; i++)
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;

        visited[u] = 1;

        printf("Pick vertex %d (dist=%d)\n", u, dist[u]);

        for (int v = 0; v < n; v++) {
            if (graph[u][v] && dist[u] != INT_MAX) {
                if (dist[v] > dist[u] + graph[u][v]) {
                    dist[v] = dist[u] + graph[u][v];
                    printf(" Update dist[%d] = %d\n", v, dist[v]);
                }
            }
        }
    }

    printf("\nFinal shortest paths from %d:\n", src);
    for (int i = 0; i < n; i++)
        printf("To %d = %d\n", i, dist[i]);
}


// ---------------------------- PRIM ----------------------------
void prim() {
    int parent[MAX], key[MAX], visited[MAX];

    for (int i = 0; i < n; i++) {
        key[i] = INT_MAX;
        visited[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    printf("\nPrim's Algorithm Trace:\n");

    for (int count = 0; count < n - 1; count++) {
        int u = -1;

        for (int i = 0; i < n; i++)
            if (!visited[i] && (u == -1 || key[i] < key[u]))
                u = i;

        visited[u] = 1;

        printf("Select vertex %d with key = %d\n", u, key[u]);

        for (int v = 0; v < n; v++) {
            if (graph[u][v] && !visited[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
                printf(" Update edge %d-%d cost = %d\n", u, v, graph[u][v]);
            }
        }
    }

    printf("\nMinimum Spanning Tree (Prim):\n");
    for (int i = 1; i < n; i++)
        printf("%d - %d : %d\n", parent[i], i, graph[i][parent[i]]);
}


// ---------------------------- KRUSKAL ----------------------------
typedef struct {
    int u, v, w;
} Edge;

int find(int parent[], int i) {
    while (parent[i] != i)
        i = parent[i];
    return i;
}

void union_set(int parent[], int x, int y) {
    parent[y] = x;
}

void kruskal() {
    Edge edges[MAX * MAX];
    int ecount = 0;

    // Convert adjacency matrix to edge list (avoid duplicates)
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (graph[i][j])
                edges[ecount++] = (Edge){i, j, graph[i][j]};

    // Sort by weight
    for (int i = 0; i < ecount; i++)
        for (int j = 0; j < ecount - 1; j++)
            if (edges[j].w > edges[j + 1].w) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }

    int parent[MAX];
    for (int i = 0; i < n; i++)
        parent[i] = i;

    printf("\nKruskal Algorithm Trace:\n");

    printf("Sorted edges:\n");
    for (int i = 0; i < ecount; i++)
        printf("(%d-%d) = %d\n", edges[i].u, edges[i].v, edges[i].w);

    printf("\nEdges Selected for MST:\n");
    for (int i = 0; i < ecount; i++) {
        int pu = find(parent, edges[i].u);
        int pv = find(parent, edges[i].v);

        if (pu != pv) {
            printf("Take edge %d-%d = %d\n", edges[i].u, edges[i].v, edges[i].w);
            union_set(parent, pu, pv);
        }
    }
}


// ---------------------------- MAIN ----------------------------
int main() {
    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (0 = no edge):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int w;
            printf("Matrix[%d][%d]: ",i,j);
            scanf("%d", &w);
            graph[i][j] = w;

            if (w != 0)
                graph[j][i] = w;
        }
    }

    printAdjMatrix();   

    int src;
    printf("Enter source for Dijkstra: ");
    scanf("%d", &src);

    if (!validate_vertex(src)) {
        printf("Invalid source vertex.\n");
        return 0;
    }

    dijkstra(src);
    prim();
    kruskal();

    return 0;
}
