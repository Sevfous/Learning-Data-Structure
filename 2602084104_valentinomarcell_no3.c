//Nama: Valentino Marcell
//NIM: 2602084104
#include <stdio.h>
#include <stdlib.h>

struct treeNode { 
    int data; 
    struct treeNode* left;
    struct treeNode* right;
};

struct treeNode* createNode(int data) { 
    struct treeNode* newNode = (struct treeNode*)malloc(sizeof(struct treeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct treeNode* insert(struct treeNode* root, int data) { 
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }

    return root;
}

struct treeNode* minValueNode(struct treeNode* node) {
    struct treeNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct treeNode* deleteNode(struct treeNode* root, int data) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        if (root->left == NULL) {
            struct treeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct treeNode* temp = root->left;
            free(root);
            return temp;
        }

        struct treeNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

void preorder(struct treeNode* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(struct treeNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void postorder(struct treeNode* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int main() {
    struct treeNode* root = NULL; 
    int menu, data, deleteData;

    while(1) {
        printf("1. Insertion\n");
        printf("2. Deletion\n");
        printf("3. Traversal\n");
        printf("4. Exit\n");
        printf("Choose: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                printf("Insert: ");
                scanf("%d", &data);
                root = insert(root, data);
                system("pause");
                system("cls");
                break;
            case 2:
                printf("Delete: ");
                scanf("%d", &deleteData);
                root = deleteNode(root, deleteData);
                system("pause");
                system("cls");
                break;
            case 3:
                printf("Preorder: ");
                preorder(root);
                printf("\n");
                printf("Inorder: ");
                inorder(root);
                printf("\n");
                printf("Postorder: ");
                postorder(root);
                printf("\n");
                system("pause");
                system("cls");
                break;
            case 4:
            	printf("Thank you\n");
            	exit(0);
        }
    } while (menu != 4);

    return 0;
}

