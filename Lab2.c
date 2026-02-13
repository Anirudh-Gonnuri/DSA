#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

// ----------- NORMAL QUEUE (Linked List) -----------
struct normalNode {
    int userId;
    struct normalNode* next;
};

struct normalNode *nFront = NULL, *nRear = NULL;

int isNormalQueueEmpty() {
    return nFront == NULL;
}

void enqueue_normal(int id) {
    struct normalNode* temp = (struct normalNode*)malloc(sizeof(struct normalNode));
    temp->userId = id;
    temp->next = NULL;
    if (nRear == NULL) {
        nFront = nRear = temp;
    } else {
        nRear->next = temp;
        nRear = temp;
    }
    printf("User %d added to workout queue.\nTime Complexity: O(1)\nSpace Complexity: O(n)\n", id);
}

void dequeue_normal() {
    if (isNormalQueueEmpty()) {
        printf("Queue Underflow — No users in workout queue.\n");
        return;
    }
    struct normalNode* temp = nFront;
    printf("User %d completed workout session.\nTime Complexity: O(1)\nSpace Complexity: O(n)\n", temp->userId);
    nFront = nFront->next;
    if (nFront == NULL)
        nRear = NULL;
    free(temp);
}

void display_normal() {
    if (isNormalQueueEmpty()) {
        printf("No users waiting in the workout queue.\n");
        return;
    }
    printf("Workout Queue: ");
    struct normalNode* temp = nFront;
    while (temp != NULL) {
        printf("%d -> ", temp->userId);
        temp = temp->next;
    }
    printf("NULL\nTime Complexity: O(n)\nSpace Complexity: O(n)\n");
}


// ----------- CIRCULAR QUEUE (Linked List) -----------
struct circularNode {
    int userId;
    struct circularNode* next;
};

struct circularNode* cRear = NULL;

int isCircularQueueEmpty() {
    return cRear == NULL;
}

void enqueue_circular(int id) {
    struct circularNode* temp = (struct circularNode*)malloc(sizeof(struct circularNode));
    temp->userId = id;
    if (cRear == NULL) {
        cRear = temp;
        cRear->next = cRear;
    } else {
        temp->next = cRear->next;
        cRear->next = temp;
        cRear = temp;
    }
    printf("User %d added to circular hydration reminder queue.\nTime Complexity: O(1)\nSpace Complexity: O(n)\n", id);
}

void dequeue_circular() {
    if (isCircularQueueEmpty()) {
        printf("Circular Queue Underflow — No hydration reminders pending.\n");
        return;
    }
    struct circularNode* temp = cRear->next;
    printf("Hydration reminder sent to user %d.\nTime Complexity: O(1)\nSpace Complexity: O(n)\n", temp->userId);
    if (cRear == temp) {
        cRear = NULL;
    } else {
        cRear->next = temp->next;
    }
    free(temp);
}

void display_circular() {
    if (isCircularQueueEmpty()) {
        printf("Circular Queue is empty.\n");
        return;
    }
    struct circularNode* temp = cRear->next;
    printf("Circular Queue: ");
    do {
        printf("%d -> ", temp->userId);
        temp = temp->next;
    } while (temp != cRear->next);
    printf("(back to start)\nTime Complexity: O(n)\nSpace Complexity: O(n)\n");
}


// ----------- PRIORITY QUEUE  -----------
struct priorityNode {
    char name[30];
    char cat[30];
    int priority;
    struct priorityNode* next;
};

struct priorityNode* pFront = NULL;

int isPriorityEmpty() {
    return pFront == NULL;
}

void enqueue_priority(char name[], char cat[], int priority) {
    struct priorityNode* temp = (struct priorityNode*)malloc(sizeof(struct priorityNode));
    strcpy(temp->name, name);
    strcpy(temp->cat, cat);
    temp->priority = priority;
    temp->next = NULL;

    if (pFront == NULL || priority < pFront->priority) {
        temp->next = pFront;
        pFront = temp;
    } else {
        struct priorityNode* current = pFront;
        while (current->next != NULL && current->next->priority <= priority)
            current = current->next;
        temp->next = current->next;
        current->next = temp;
    }
    printf("Added %s (%s) with priority %d.\nTime Complexity: O(n)\nSpace Complexity: O(n)\n", name, cat, priority);
}

void dequeue_priority() {
    if (isPriorityEmpty()) {
        printf("Priority Queue Empty.\n");
        return;
    }
    struct priorityNode* temp = pFront;
    printf("Served %s (%s) with priority %d.\nTime Complexity: O(1)\nSpace Complexity: O(n)\n", temp->name, temp->cat, temp->priority);
    pFront = pFront->next;
    free(temp);
}

void display_priority() {
    if (isPriorityEmpty()) {
        printf("Priority Queue empty.\n");
        return;
    }
    struct priorityNode* temp = pFront;
    printf("Priority Queue (High Priority First):\n");
    while (temp != NULL) {
        printf("%s (%s) -> Priority: %d\n", temp->name, temp->cat, temp->priority);
        temp = temp->next;
    }
    printf("Time Complexity: O(n)\nSpace Complexity: O(n)\n");
}


// ----------- MAIN MENU -----------
int main() {
    int ch, ich, id, priority;
    char name[30], cat[30];

    while (1) {
        printf("\n=== Fitness Tracker Queue System ===\n");
        printf("1. Normal Queue (Workout Sessions)\n");
        printf("2. Circular Queue (Hydration Reminders)\n");
        printf("3. Priority Queue (Emergency Requests)\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        if (ch == 1) {
            do {
                printf("\n-- Normal Queue Operations --\n");
                printf("1. Enqueue (New user)\n2. Dequeue (Workout done)\n3. Display\n4. Exit menu\nEnter: ");
                scanf("%d", &ich);
                if (ich == 1) {
                    printf("Enter User ID: ");
                    scanf("%d", &id);
                    enqueue_normal(id);
                } else if (ich == 2) {
                    dequeue_normal();
                } else if (ich == 3) {
                    display_normal();
                }
            } while (ich != 4);
        }

        else if (ch == 2) {
            do {
                printf("\n-- Circular Queue Operations --\n");
                printf("1. Enqueue (Add reminder)\n2. Dequeue (Send reminder)\n3. Display\n4. Exit menu\nEnter: ");
                scanf("%d", &ich);
                if (ich == 1) {
                    printf("Enter User ID: ");
                    scanf("%d", &id);
                    enqueue_circular(id);
                } else if (ich == 2) {
                    dequeue_circular();
                } else if (ich == 3) {
                    display_circular();
                }
            } while (ich != 4);
        }

        else if (ch == 3) {
            do {
                printf("\n-- Priority Queue Operations --\n");
                printf("1. Enqueue (Emergency Request)\n2. Dequeue (Serve Request)\n3. Display\n4. Exit menu\nEnter: ");
                scanf("%d", &ich);
                if (ich == 1) {
                    printf("Enter Name: ");
                    scanf("%s", name);
                    printf("Enter Category (e.g., Trainer/Member): ");
                    scanf("%s", cat);
                    printf("Enter Priority (1 = High, higher number = lower priority): ");
                    scanf("%d", &priority);
                    enqueue_priority(name, cat, priority);
                } else if (ich == 2) {
                    dequeue_priority();
                } else if (ich == 3) {
                    display_priority();
                }
            } while (ich != 4);
        }

        else if (ch == 4) {
            printf("Exiting Fitness Tracker Queue System.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
