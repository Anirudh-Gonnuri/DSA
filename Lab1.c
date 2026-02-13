#include <stdio.h>

#define MAX 30

int mainSteps[MAX], minStack[MAX];
int top = -1, minTop = -1;

void push(int steps) {
    if (top + 1 == MAX) {
        printf("Error: Stack Overflow!\n");
        return;
    }
    top++;
    mainSteps[top] = steps;

    if (minTop == -1 || steps <= minStack[minTop]) {
        minTop++;
        minStack[minTop] = steps;
    }

    printf("Operation: Push(%d)\n", steps);
    printf("Current Top: %d\n", mainSteps[top]);
    printf("Current Min: %d\n", minStack[minTop]);
}

void pop() {
    if (top == -1) {
        printf("Error: Stack Underflow! No steps to remove.\n");
        return;
    }
    int removed = mainSteps[top];

    if (removed == minStack[minTop]) {
        minTop--;
    }
    top--;

    printf("Operation: Pop(%d)\n", removed);
    if (top != -1) {
        printf("Current Top: %d\n", mainSteps[top]);
        printf("Current Min: %d\n", minStack[minTop]);
    } else {
        printf("Stack is now empty.\n");
    }
}

void peek() {
    if (top == -1) {
        printf("Error: Stack is empty! No data.\n");
        return;
    }
    printf("Yesterday's Step Count: %d\n", mainSteps[top]);
}

void getMin() {
    if (minTop == -1) {
        printf("Error: No data in stack!\n");
        return;
    }
    printf("Lowest Step Count Recorded: %d\n", minStack[minTop]);
}

int main() {
    int ch, steps;

    printf("=== Daily Step Tracker (MinStack) ===\n");
    printf("1. Enter today's steps\n");
    printf("2. Fetch yesterday's step count (peek)\n");
    printf("3. Show lowest step count (getMin)\n");
    printf("4. Remove last step count entry (pop)\n");
    printf("5. Exit\n");

    while (1) {
        printf("\nEnter Choice: ");
        scanf("%d",&ch);
        switch (ch) {
            case 1:
                printf("Enter steps: ");
                scanf("%d", &steps);
                if (steps < 0) {
                    printf("Invalid steps! Must be >= 0.\n");
                } else {
                    push(steps);
                    
                }
                break;

            case 2:
                peek();
                break;

            case 3:
                getMin();
                break;

            case 4:
                pop();
                break;

            case 5:
                printf("Exiting Step Tracker...\n");
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}
