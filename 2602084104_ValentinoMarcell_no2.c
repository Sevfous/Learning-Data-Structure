//Nama: Valentino Marcell
//NIM: 2602084104
#include <stdio.h>
#include <stdlib.h>

enum Color { RED, BLACK };

struct Node {
    int data;
    enum Color color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
};

struct Node* root = NULL;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->color = RED;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void swapColors(enum Color* a, enum Color* b) {
    enum Color temp = *a;
    *a = *b;
    *b = temp;
}

struct Node* bstInsert(struct Node* root, struct Node* newNode) {
    if (root == NULL)
        return newNode;

    if (newNode->data < root->data) {
        root->left = bstInsert(root->left, newNode);
        root->left->parent = root;
    } else if (newNode->data > root->data) {
        root->right = bstInsert(root->right, newNode);
        root->right->parent = root;
    }

    return root;
}

void fixViolation(struct Node* root, struct Node* newNode) {
    struct Node* parent = NULL;
    struct Node* grandparent = NULL;

    while ((newNode != root) && (newNode->color != BLACK) &&
           (newNode->parent->color == RED)) {
        parent = newNode->parent;
        grandparent = newNode->parent->parent;

        if (parent == grandparent->left) {
            struct Node* uncle = grandparent->right;

            if (uncle != NULL && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                newNode = grandparent;
            } else {
                if (newNode == parent->right) {
                    newNode = parent;
                    leftRotate(root, newNode);
                }

                parent = newNode->parent;
                grandparent = parent->parent;
                parent->color = BLACK;
                grandparent->color = RED;
                rightRotate(root, grandparent);
            }
        } else {
            struct Node* uncle = grandparent->left;

            if (uncle != NULL && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                newNode = grandparent;
            } else {
                if (newNode == parent->left) {
                    newNode = parent;
                    rightRotate(root, newNode);
                }

                parent = newNode->parent;
                grandparent = parent->parent;
                parent->color = BLACK;
                grandparent->color = RED;
                leftRotate(root, grandparent);
            }
        }
    }

    root->color = BLACK;
}

void leftRotate(struct Node* root, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;

    if (x->right != NULL)
        x->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(struct Node* root, struct Node* y) {
    struct Node* x = y->left;
    y->left = x->right;

    if (y->left != NULL)
        y->left->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void inorderTraversal(struct Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

int main() {
    int menu;

    while(1){
        printf("1. Insertion\n");
        printf("2. Inorder Traversal\n");
        printf("3. Exit\n");
        printf(">> ");
        scanf("%d", &menu);
        switch (menu) {
            case 1: {
                int Array[] = {41, 22, 5, 51, 48, 29, 18, 21, 45, 3};
                int Length = sizeof(Array) / sizeof(Array[0]);

                for (int i = 0; i < Length; i++) {
                    struct Node* newNode = createNode(Array[i]);
                    root = bstInsert(root, newNode);
                    fixViolation(root, newNode);
                }

                printf("[41, 22, 5, 51, 48, 29, 18, 21, 45, 3]\nInserted\n");
                break;
            }
            case 2:
                printf("Inorder Traversal: ");
                inorderTraversal(root);
                printf("\n");
                break;
            case 3:
            	exit(0);
        }
    } 

    return 0;
}

