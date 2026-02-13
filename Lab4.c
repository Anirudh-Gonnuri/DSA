#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

int adj[MAX][MAX];      // Adjacency matrix
int n = 0;              // Number of workout sessions
char sessions[MAX][30]; // Session names
int visited[MAX];       // For DFS/BFS
int recStack[MAX];      // For cycle detection

// ----------- Queue for BFT -----------
int queue[MAX], front = -1, rear = -1;

void enqueue(int v) {
    if (rear == MAX - 1) return;
    if (front == -1) front = 0;
    queue[++rear] = v;
}

int dequeue() {
    if (front == -1 || front > rear) return -1;
    return queue[front++];
}

int isEmpty() {
    return (front == -1 || front > rear);
}

// ----------- Graph Creation -----------
void createGraph() {
    printf("\nEnter number of workout sessions: ");
    scanf("%d", &n);
    getchar();

    printf("\nEnter names of workout sessions:\n");
    for (int i = 0; i < n; i++) {
        printf("Session %d: ", i);
        fgets(sessions[i], sizeof(sessions[i]), stdin);
        sessions[i][strcspn(sessions[i], "\n")] = '\0'; // Remove newline
    }

    // Initialize adjacency matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adj[i][j] = 0;

    printf("\nEnter dependencies (A -> B means A before B):\n");
    printf("Enter -1 -1 to stop.\n");

    int src, dest;
    while (1) {
        printf("Enter source and destination (indices 0 to %d): ", n - 1);
        scanf("%d%d", &src, &dest);
        if (src == -1 && dest == -1) break;
        if (src >= 0 && src < n && dest >= 0 && dest < n)
            adj[src][dest] = 1;
        else
            printf("Invalid indices. Try again.\n");
    }

    printf("\nWorkout Session Graph created successfully.\n");
}

// ----------- BFT -----------
void BFT(int start) {
    for (int i = 0; i < n; i++) visited[i] = 0;
    front = rear = -1;

    enqueue(start);
    visited[start] = 1;

    printf("Breadth-First Traversal: ");
    while (!isEmpty()) {
        int v = dequeue();
        printf("%s ", sessions[v]);
        for (int i = 0; i < n; i++) {
            if (adj[v][i] && !visited[i]) {
                enqueue(i);
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

// ----------- DFT -----------
void DFTUtil(int v) {
    visited[v] = 1;
    printf("%s ", sessions[v]);
    for (int i = 0; i < n; i++) {
        if (adj[v][i] && !visited[i])
            DFTUtil(i);
    }
}

void DFT(int start) {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("Depth-First Traversal: ");
    DFTUtil(start);
    printf("\n");
}

// ----------- Cycle Detection (Check DAG) -----------
int isCyclicUtil(int v) {
    visited[v] = 1;
    recStack[v] = 1;

    for (int i = 0; i < n; i++) {
        if (adj[v][i]) {
            if (!visited[i] && isCyclicUtil(i))
                return 1;
            else if (recStack[i])
                return 1;
        }
    }

    recStack[v] = 0;
    return 0;
}

int isCyclic() {
    for (int i = 0; i < n; i++) {
        visited[i] = recStack[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if (isCyclicUtil(i))
                return 1;
        }
    }
    return 0;
}

// ----------- Topological Sort -----------
void topoUtil(int v, int visited[], int *stack, int *top) {
    visited[v] = 1;

    for (int i = 0; i < n; i++) {
        if (adj[v][i] && !visited[i])
            topoUtil(i, visited, stack, top);
    }
    stack[++(*top)] = v;
}

void topologicalSort() {
    if (isCyclic()) {
        printf("\nCannot perform topological sort - workout plan has cyclic dependencies!\n");
        return;
    }

    int visited[MAX] = {0};
    int stack[MAX], top = -1;

    for (int i = 0; i < n; i++) {
        if (!visited[i])
            topoUtil(i, visited, stack, &top);
    }

    printf("\nValid workout session order (Topological Sort): ");
    while (top >= 0)
        printf("%s ", sessions[stack[top--]]);
    printf("\n");
}

// ----------- Main Menu -----------
int main() {
    int choice, start;

    while (1) {
        printf("\n===== FITNESS TRACKER: WORKOUT SESSION DEPENDENCY MENU =====\n");
        printf("1. Create Workout Session Graph\n");
        printf("2. Check for Cyclic Dependencies (DAG Check)\n");
        printf("3. Traversal (BFT & DFT)\n");
        printf("4. Display Valid Workout Order (Topological Sort)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createGraph();
                break;

            case 2:
                if (isCyclic())
                    printf("\nThe workout plan has cyclic dependencies (Not a DAG)\n");
                else
                    printf("\nThe workout plan is acyclic (Valid DAG)\n");
                break;

            case 3:
                printf("Enter starting session index (0 to %d): ", n - 1);
                scanf("%d", &start);
                if (start >= 0 && start < n) {
                    BFT(start);
                    DFT(start);
                } else {
                    printf("Invalid index!\n");
                }
                break;

            case 4:
                topologicalSort();
                break;

            case 5:
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
