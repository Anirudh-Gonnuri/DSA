#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100
#define INF 999999

//-------------------------------------------------------------
// Min-Heap / Priority Queue Node
//-------------------------------------------------------------
typedef struct {
    int vertex;
    int dist;
} HeapNode;

typedef struct {
    HeapNode *arr;
    int size;
} MinHeap;

// Swap function
void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Create MinHeap
MinHeap* createMinHeap(int capacity) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->arr = malloc(sizeof(HeapNode) * capacity);
    h->size = 0;
    return h;
}

// Insert into heap
void push(MinHeap *h, int vertex, int dist) {
    h->arr[h->size].vertex = vertex;
    h->arr[h->size].dist = dist;
    int i = h->size++;
    
    // Up-heapify
    while (i != 0 && h->arr[(i - 1) / 2].dist > h->arr[i].dist) {
        swap(&h->arr[i], &h->arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Extract min
HeapNode pop(MinHeap *h) {
    HeapNode root = h->arr[0];
    h->arr[0] = h->arr[--h->size];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < h->size && h->arr[left].dist < h->arr[smallest].dist)
            smallest = left;
        if (right < h->size && h->arr[right].dist < h->arr[smallest].dist)
            smallest = right;

        if (smallest == i)
            break;

        swap(&h->arr[i], &h->arr[smallest]);
        i = smallest;
    }
    return root;
}

int isEmpty(MinHeap *h) {
    return h->size == 0;
}

//-------------------------------------------------------------
// Graph (Adjacency Matrix)
//-------------------------------------------------------------
void displayGraph(int graph[100][100], int V, char stations[][30]) {
    printf("\nAdjacency Matrix (time between workout stations in minutes):\n\t");
    printf("     ");
    for (int i = 0; i < V; i++) printf("%10s", stations[i]);
    printf("\n");

    for (int i = 0; i < V; i++) {
        printf("%10s", stations[i]);
        for (int j = 0; j < V; j++) {
            printf("%10d", graph[i][j]);
        }
        printf("\n");
    }
}

//-------------------------------------------------------------
// DIJKSTRA'S ALGORITHM
//-------------------------------------------------------------
void dijkstra(int graph[MAX][MAX], int V, int src, char stations[][30]) {
    int dist[MAX];
    int visited[MAX] = {0};

    for (int i = 0; i < V; i++)
        dist[i] = INF;

    dist[src] = 0;

    MinHeap *pq = createMinHeap(V);
    push(pq, src, 0);

    printf("\n--- Dijkstra Tracing ---\n");

    while (!isEmpty(pq)) {
        HeapNode temp = pop(pq);
        int u = temp.vertex;

        if (visited[u])
            continue;

        visited[u] = 1;

        printf("\nSelected Station: %s (current dist = %d)\n", stations[u], dist[u]);

        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && !visited[v]) {
                int newDist = dist[u] + graph[u][v];

                printf("Checking -> %s -> %s ", stations[u], stations[v]);
                printf(" | Weight = %d | NewDist = %d | OldDist = %d\n",
                       graph[u][v], newDist, dist[v]);

                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    printf("Updated dist[%s] = %d\n", stations[v], newDist);
                    push(pq, v, newDist);
                }
            }
        }
    }

    printf("\n--- Final Shortest Times from %s ---\n", stations[src]);
    for (int i = 0; i < V; i++) {
        printf("To %-10s : %d minutes\n", stations[i], dist[i]);
    }
}

//-------------------------------------------------------------
// MAIN (Fitness Domain Shortest Path)
//-------------------------------------------------------------
int main() {
    int V = 6;
    // User-defined station names
    char stations[6][30];

    printf("Enter the names of 6 workout stations:\n");
    for (int i = 0; i < 6; i++) {
        printf("Station %d: ", i + 1);
        scanf("%s", stations[i]);   // one-word names
    }

    // User-defined adjacency matrix
    int graph[MAX][MAX];

    printf("\nEnter time (in minutes) between stations.\n");
    printf("Enter 0 if no direct path exists.\n\n");

    // Fill upper triangle and mirror it
    for (int i = 0; i < 6; i++) {
        graph[i][i] = 0;  // diagonal always 0

        for (int j = i + 1; j < 6; j++) {
            printf("Time between %s and %s: ", stations[i], stations[j]);
            scanf("%d", &graph[i][j]);

            graph[j][i] = graph[i][j];  // make symmetric
        }
    }

    // (Optional) Print matrix to confirm
    printf("\nFinal adjacency matrix:\n");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%4d", graph[i][j]);
        }
        printf("\n");
    }

    int loop=0;
    do{
    displayGraph(graph, V, stations);
    int src;
    printf("\nEnter starting station index (0-5): ");
    scanf("%d", &src);

    if (src < 0 || src >= V) {
        printf("Invalid source station.\n");
        return 0;
    }

    dijkstra(graph, V, src, stations);
    printf("\nRepeat? Press 1: ");
    scanf("%d",&loop);
    }while(loop==1);
    return 0;
}

