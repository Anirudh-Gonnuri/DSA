#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if the string is valid (not empty and alphanumeric)
int isValidString(char *s) {
    if(strlen(s) == 0) return 0;
    for(int i = 0; s[i] != '\0'; i++){
        if(!isalnum(s[i])) return 0;
    }
    return 1;
}

// Function to get LCS string by tracing back the DP table
void printLCS(char **trace, char *X, int i, int j) {
    if(i == 0 || j == 0)
        return;

    if(trace[i][j] == 'D') {  // Diagonal match
        printLCS(trace, X, i-1, j-1);
        printf("%c", X[i-1]);
    }
    else if(trace[i][j] == 'U')
        printLCS(trace, X, i-1, j);
    else
        printLCS(trace, X, i, j-1);
}

int main() {
    char *X, *Y;
    int m, n, i, j;

    // Allocating dynamic memory for strings
    X = (char*) malloc(100 * sizeof(char));
    Y = (char*) malloc(100 * sizeof(char));

    printf("Enter first string: ");
    scanf("%s", X);

    printf("Enter second string: ");
    scanf("%s", Y);

    // Validation
    if(!isValidString(X) || !isValidString(Y)) {
        printf("Invalid input strings! Only non-empty alphanumeric values allowed.\n");
        return 0;
    }

    m = strlen(X);
    n = strlen(Y);

    // Allocating DP table
    int **L = (int **) malloc((m+1) * sizeof(int*));
    for(i = 0; i <= m; i++)
        L[i] = (int *) malloc((n+1) * sizeof(int));

    // Trace direction table
    char **trace = (char **) malloc((m+1) * sizeof(char*));
    for(i = 0; i <= m; i++)
        trace[i] = (char *) malloc((n+1) * sizeof(char));

    // Building LCS DP table
    for(i = 0; i <= m; i++) {
        for(j = 0; j <= n; j++) {
            if(i == 0 || j == 0)
                L[i][j] = 0;
            else if(X[i-1] == Y[j-1]) {
                L[i][j] = L[i-1][j-1] + 1;
                trace[i][j] = 'D';   // Diagonal
            } else {
                if(L[i-1][j] >= L[i][j-1]) {
                    L[i][j] = L[i-1][j];
                    trace[i][j] = 'U';   // Up
                } else {
                    L[i][j] = L[i][j-1];
                    trace[i][j] = 'L';   // Left
                }
            }
        }
    }

    // Display DP Table
    printf("\nLCS DP Table:\n");
    for(i = 0; i <= m; i++) {
        for(j = 0; j <= n; j++) {
            printf("%2d ", L[i][j]);
        }
        printf("\n");
    }

    // LCS length
    printf("\nLength of LCS: %d\n", L[m][n]);

    // Printing any one valid LCS
    printf("One valid LCS: ");
    printLCS(trace, X, m, n);
    printf("\n");

    // Complexity Discussion
    printf("\nComplexity Discussion:\n");
    printf("Time Complexity: O(m * n)\n");
    printf("Space Complexity: O(m * n)\n");

    // Free memory
    for(i = 0; i <= m; i++) {
        free(L[i]);
        free(trace[i]);
    }
    free(L);
    free(trace);
    free(X);
    free(Y);

    return 0;
}
