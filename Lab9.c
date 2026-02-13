#include <stdio.h>
#include <limits.h>

#define MAX 10
#define INF 999999

int n;                      // number of cities
int dist[MAX][MAX];         // distance matrix
int dp[1<<MAX][MAX];
int parent[1<<MAX][MAX];

int min(int a, int b) {
    return (a < b) ? a : b;
}

int main() {
    int i, j;

    printf("Enter number of cities (min 4): ");
    scanf("%d", &n);

    if(n < 4 || n > MAX) {
        printf("Invalid number of cities!\n");
        return 0;
    }

    printf("\nEnter the distance matrix (%d x %d):\n", n, n);
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &dist[i][j]);
        }
    }

    int VISITED_ALL = (1<<n) - 1;

    // Initialize DP table
    for(int mask = 0; mask < (1<<n); mask++) {
        for(int city = 0; city < n; city++) {
            dp[mask][city] = INF;
        }
    }

    dp[1][0] = 0; // starting from city 0

    // DP Masking Loop
    for(int mask = 0; mask < (1<<n); mask++) {
        for(int u = 0; u < n; u++) {
            if(mask & (1<<u)) {
                for(int v = 0; v < n; v++) {
                    if(!(mask & (1<<v))) {
                        int newMask = mask | (1<<v);
                        int newCost = dp[mask][u] + dist[u][v];
                        
                        if(newCost < dp[newMask][v]) {
                            dp[newMask][v] = newCost;
                            parent[newMask][v] = u;
                        }
                    }
                }
            }
        }
    }

    // Compute min cost returning to starting city
    int minCost = INF;
    int lastCity = -1;

    for(int i = 1; i < n; i++) {
        int cost = dp[VISITED_ALL][i] + dist[i][0];
        if(cost < minCost) {
            minCost = cost;
            lastCity = i;
        }
    }

    printf("\nMinimum Tour Cost: %d\n", minCost);

    // Reconstruct the optimal path
    int mask = VISITED_ALL;
    int city = lastCity;
    int tour[MAX+1];
    int idx = n - 1;

    while(city != 0) {
        tour[idx--] = city;
        int temp = parent[mask][city];
        mask = mask ^ (1<<city);
        city = temp;
    }

    tour[0] = 0;
    tour[n] = 0;

    printf("Tour: ");
    for(i = 0; i <= n; i++) {
        printf("%d ", tour[i]);
    }
    printf("\n");

    // Display DP Table
    printf("\nDP Table (Intermediate Values):\n");
    for(int m = 0; m < (1<<n); m++) {
        for(int c = 0; c < n; c++) {
            if(dp[m][c] == INF)
                printf("INF\t");
            else
                printf("%d\t", dp[m][c]);
        }
        printf("\n");
    }

    // Complexity section
    printf("\nComplexity Discussion:\n");
    printf("Time Complexity: O(n^2 * 2^n)\n");
    printf("Space Complexity: O(n * 2^n)\n");

    return 0;
}
