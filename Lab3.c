#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Domain data structure --- */
typedef struct FitnessData {
    int user_id;         // key
    char name[31];       // user name (max 30 chars)
    int steps;           // daily steps
} FitnessData;

/* --- Node structure --- */
typedef struct Node {
    FitnessData data;
    struct Node *left;
    struct Node *right;
    int height; // height (used for AVL).
} Node;

/* --- Utility functions --- */
int max(int a, int b) { return (a > b) ? a : b; }

int height(Node *n) {
    if (!n) return 0;
    return n->height;
}

Node* createNode(int user_id, const char *name, int steps) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    n->data.user_id = user_id;
    strncpy(n->data.name, name, 30);
    n->data.name[30] = '\0';
    n->data.steps = steps;
    n->left = n->right = NULL;
    n->height = 1; // leaf node height 1
    return n;
}

/* --- BST functions --- */

/* Insert into BST (non-balancing). If key exists, update payload */
Node* bstInsert(Node *root, int user_id, const char *name, int steps) {
    if (root == NULL) {
        return createNode(user_id, name, steps);
    }
    if (user_id < root->data.user_id) {
        root->left = bstInsert(root->left, user_id, name, steps);
    } else if (user_id > root->data.user_id) {
        root->right = bstInsert(root->right, user_id, name, steps);
    } else {
        // key exists -> update payload
        strncpy(root->data.name, name, 30);
        root->data.name[30] = '\0';
        root->data.steps = steps;
    }
    // height field not maintained for BST
    return root;
}

Node* bstMinValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

/* Delete node from BST by key */
Node* bstDelete(Node* root, int user_id) {
    if (root == NULL) return root;

    if (user_id < root->data.user_id) {
        root->left = bstDelete(root->left, user_id);
    } else if (user_id > root->data.user_id) {
        root->right = bstDelete(root->right, user_id);
    } else {
        // node found
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        } else {
            // two children: get inorder successor
            Node* temp = bstMinValueNode(root->right);
            root->data = temp->data;
            root->right = bstDelete(root->right, temp->data.user_id);
        }
    }
    return root;
}

/* Search in BST */
Node* bstSearch(Node* root, int user_id) {
    if (root == NULL) return NULL;
    if (user_id == root->data.user_id) return root;
    if (user_id < root->data.user_id) return bstSearch(root->left, user_id);
    return bstSearch(root->right, user_id);
}

/* Traversals */
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    printf("ID: %d | Name: %s | Steps: %d\n", root->data.user_id, root->data.name, root->data.steps);
    inorder(root->right);
}

void preorder(Node* root) {
    if (!root) return;
    printf("ID: %d | Name: %s | Steps: %d\n", root->data.user_id, root->data.name, root->data.steps);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    printf("ID: %d | Name: %s | Steps: %d\n", root->data.user_id, root->data.name, root->data.steps);
}

/* --- AVL functions --- */

int getBalance(Node* n) {
    if (!n) return 0;
    return height(n->left) - height(n->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // rotate
    x->right = y;
    y->left = T2;

    // update heights
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x; // new root
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // rotate
    y->left = x;
    x->right = T2;

    // update heights
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y; // new root
}

/* Insert into AVL and balance */
Node* avlInsert(Node* root, int user_id, const char *name, int steps) {
    if (root == NULL)
        return createNode(user_id, name, steps);

    if (user_id < root->data.user_id)
        root->left = avlInsert(root->left, user_id, name, steps);
    else if (user_id > root->data.user_id)
        root->right = avlInsert(root->right, user_id, name, steps);
    else {
        // key exists, update payload
        strncpy(root->data.name, name, 30);
        root->data.name[30] = '\0';
        root->data.steps = steps;
        return root;
    }

    // update height
    root->height = 1 + max(height(root->left), height(root->right));

    // get balance factor
    int balance = getBalance(root);

    // Left Left
    if (balance > 1 && user_id < root->left->data.user_id)
        return rightRotate(root);

    // Right Right
    if (balance < -1 && user_id > root->right->data.user_id)
        return leftRotate(root);

    // Left Right
    if (balance > 1 && user_id > root->left->data.user_id) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left
    if (balance < -1 && user_id < root->right->data.user_id) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* Delete from AVL */
Node* avlDelete(Node* root, int user_id) {
    if (root == NULL) return root;

    if (user_id < root->data.user_id) {
        root->left = avlDelete(root->left, user_id);
    } else if (user_id > root->data.user_id) {
        root->right = avlDelete(root->right, user_id);
    } else {
        // node found
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                // no child
                temp = root;
                root = NULL;
            } else {
                // one child: copy contents
                *root = *temp;
            }
            free(temp);
        } else {
            // two children: get inorder successor
            Node* temp = bstMinValueNode(root->right);
            root->data = temp->data;
            root->right = avlDelete(root->right, temp->data.user_id);
        }
    }

    if (root == NULL) return root;

    // update height
    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* Search for AVL is same as BST */
Node* avlSearch(Node* root, int user_id) {
    return bstSearch(root, user_id);
}

/* --- Helpers for menus & I/O --- */

void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) ;
}

void promptInsertAndRead(int *user_id, char *name, int *steps) {
    printf("Enter user ID (integer): ");
    while (scanf("%d", user_id) != 1) {
        printf("Invalid. Enter an integer user ID: ");
        flushInput();
    }
    flushInput();
    printf("Enter name (no spaces, max 30 chars): ");
    if (scanf("%30s", name) != 1) {
        name[0] = '\0';
    }
    printf("Enter steps (integer): ");
    while (scanf("%d", steps) != 1) {
        printf("Invalid. Enter an integer for steps: ");
        flushInput();
    }
    flushInput();
}

void displayTraversalMenuAndShow(Node* root) {
    if (!root) {
        printf("Tree is empty.\n");
        return;
    }
    int choice;
    printf("Display options:\n1. Inorder\n2. Preorder\n3. Postorder\nChoose: ");
    if (scanf("%d", &choice) != 1) {
        flushInput();
        printf("Invalid choice\n");
        return;
    }
    flushInput();
    switch (choice) {
        case 1:
            printf("Inorder traversal:\n");
            inorder(root);
            break;
        case 2:
            printf("Preorder traversal:\n");
            preorder(root);
            break;
        case 3:
            printf("Postorder traversal:\n");
            postorder(root);
            break;
        default:
            printf("Invalid option\n");
    }
}

/* --- Comparison printing --- */
void printComplexityComparison() {
    printf("\nTime Complexity Comparison (theoretical):\n");
    printf("Operation     | BST (average) | BST (worst)   | AVL (always)\n");
    printf("------------------------------------------------------------\n");
    printf("Search        | O(log n)      | O(n)          | O(log n)\n");
    printf("Insert        | O(log n)      | O(n)          | O(log n)\n");
    printf("Delete        | O(log n)      | O(n)          | O(log n)\n");
    printf("Space         | O(n)          | O(n)          | O(n)\n");
    printf("\nNotes:\n- BST worst-case occurs when it degenerates into a linked list (sorted inserts).\n- AVL maintains height-balanced property, guaranteeing O(log n) operations.\n\n");
}

/* --- Menu driver functions --- */

void bstMenu(Node **root) {
    while (1) {
        printf("\n--- BST Operations Menu ---\n");
        printf("1. Insert <user_id, name, steps>\n");
        printf("2. Delete <user_id>\n");
        printf("3. Search <user_id>\n");
        printf("4. Display (inorder, preorder, postorder)\n");
        printf("5. Back to Main Menu\n");
        printf("Choose: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            flushInput();
            printf("Invalid input\n");
            continue;
        }
        flushInput();
        if (choice == 1) {
            int id, steps;
            char name[31];
            promptInsertAndRead(&id, name, &steps);
            *root = bstInsert(*root, id, name, steps);
            printf("Inserted/Updated user ID %d into BST.\n", id);
        } else if (choice == 2) {
            int id;
            printf("Enter user ID to delete: ");
            if (scanf("%d", &id) != 1) {
                flushInput();
                printf("Invalid ID.\n");
                continue;
            }
            flushInput();
            Node *found = bstSearch(*root, id);
            if (!found) {
                printf("User ID %d not found in BST.\n", id);
            } else {
                *root = bstDelete(*root, id);
                printf("Deleted user ID %d from BST.\n", id);
            }
        } else if (choice == 3) {
            int id;
            printf("Enter user ID to search: ");
            if (scanf("%d", &id) != 1) {
                flushInput();
                printf("Invalid ID.\n");
                continue;
            }
            flushInput();
            Node *res = bstSearch(*root, id);
            if (res) {
                printf("Found: ID: %d | Name: %s | Steps: %d\n", res->data.user_id, res->data.name, res->data.steps);
            } else {
                printf("User ID %d not found in BST.\n", id);
            }
        } else if (choice == 4) {
            displayTraversalMenuAndShow(*root);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

void avlMenu(Node **root) {
    while (1) {
        printf("\n--- AVL Operations Menu ---\n");
        printf("1. Insert <user_id, name, steps>\n");
        printf("2. Delete <user_id>\n");
        printf("3. Search <user_id>\n");
        printf("4. Display (inorder, preorder, postorder)\n");
        printf("5. Back to Main Menu\n");
        printf("Choose: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            flushInput();
            printf("Invalid input\n");
            continue;
        }
        flushInput();
        if (choice == 1) {
            int id, steps;
            char name[31];
            promptInsertAndRead(&id, name, &steps);
            *root = avlInsert(*root, id, name, steps);
            printf("Inserted/Updated user ID %d into AVL tree.\n", id);
        } else if (choice == 2) {
            int id;
            printf("Enter user ID to delete: ");
            if (scanf("%d", &id) != 1) {
                flushInput();
                printf("Invalid ID.\n");
                continue;
            }
            flushInput();
            Node *found = avlSearch(*root, id);
            if (!found) {
                printf("User ID %d not found in AVL.\n", id);
            } else {
                *root = avlDelete(*root, id);
                printf("Deleted user ID %d from AVL tree.\n", id);
            }
        } else if (choice == 3) {
            int id;
            printf("Enter user ID to search: ");
            if (scanf("%d", &id) != 1) {
                flushInput();
                printf("Invalid ID.\n");
                continue;
            }
            flushInput();
            Node *res = avlSearch(*root, id);
            if (res) {
                printf("Found: ID: %d | Name: %s | Steps: %d\n", res->data.user_id, res->data.name, res->data.steps);
            } else {
                printf("User ID %d not found in AVL.\n", id);
            }
        } else if (choice == 4) {
            displayTraversalMenuAndShow(*root);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* Free tree memory */
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/* --- Main --- */
int main() {
    Node *bstRoot = NULL;
    Node *avlRoot = NULL;

    while (1) {
        printf("\n=== Fitness Tracker: BST & AVL Menu ===\n");
        printf("1. Work with Binary Search Tree (BST)\n");
        printf("2. Work with AVL Tree\n");
        printf("3. Compare Time Complexities (BST vs AVL)\n");
        printf("4. Exit\n");
        printf("Choose: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            flushInput();
            printf("Invalid input, try again.\n");
            continue;
        }
        flushInput();

        if (choice == 1) {
            bstMenu(&bstRoot);
        } else if (choice == 2) {
            avlMenu(&avlRoot);
        } else if (choice == 3) {
            printComplexityComparison();
        } else if (choice == 4) {
            printf("Exiting. Cleaning up memory...\n");
            freeTree(bstRoot);
            freeTree(avlRoot);
            break;
        } else {
            printf("Invalid choice. Please choose 1-4.\n");
        }
    }

    return 0;
}
