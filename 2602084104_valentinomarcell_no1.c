#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
    char *keys;
    int degree;
    struct node **child;
    int numKey;
    bool leaf;
};

//For main root
struct root_node{
    struct node *root;
    int degree;
};

struct root_node *construct_b_tree(int degree){
    struct root_node *b_tree = (struct root_node*)malloc(sizeof(struct root_node));
    b_tree->root = NULL;
    b_tree->degree = degree;
    return b_tree;
}

struct node *create_node(struct root_node *b_tree, int degree, bool leaf){
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->degree = degree;
    node->leaf = leaf;
    node->keys = malloc((sizeof(char) * b_tree->degree * 2) - 1);
    node->child = malloc((sizeof(struct node) * b_tree->degree * 2));
    node->numKey = 0;
    return node;
}

void split_child(struct node *root_node, int idx, struct node *child, struct root_node *b_tree){
    struct node *newnode = create_node(b_tree, child->degree, child->leaf);
    newnode->numKey = root_node->degree - 1;
    for (int j = 0; j < root_node->degree - 1; j++) newnode->keys[j] = child->keys[j + root_node->degree];

    if (child->leaf == false){
        for (int j = 0; j < root_node->degree; j++) newnode->child[j] = child->child[j + root_node->degree];
    }
    child->numKey = root_node->degree - 1;
    for (int j = root_node->numKey; j >= idx + 1; j--) root_node->child[j + 1] = root_node->child[j];

    root_node->child[idx + 1] = newnode;
    for (int j = root_node->numKey - 1; j >= idx; j--) root_node->keys[j + 1] = root_node->keys[j];

    root_node->keys[idx] = child->keys[root_node->degree - 1]; root_node->numKey ++;
}

void insert_node_not_full(struct root_node *b_tree, struct node *root_node, char key){
    int i = root_node->numKey - 1;
    if (root_node->leaf == true){
        while (i >= 0 && root_node->keys[i] > key){
            root_node->keys[i + 1] = root_node->keys[i];
            i--;
        }
        root_node->keys[i + 1] = key; root_node->numKey ++;
    }else{
        while (i >= 0 && root_node->keys[i] > key) i--;

        if (root_node->child[i + 1]->numKey == (2 * root_node->degree) - 1){
            split_child(root_node, i + 1, root_node->child[i + 1], b_tree);
            if (root_node->keys[i + 1] < key) i++;
        }
        insert_node_not_full(b_tree, root_node->child[i + 1], key);
    }
}

void insert_node(struct root_node *b_tree, char key){
    if (b_tree->root == NULL){
        b_tree->root = create_node(b_tree, b_tree->degree, true);
        b_tree->root->keys[0] = key;
        b_tree->root->numKey = 1;
    }else{
        if (b_tree->root->numKey == (2 * b_tree->degree) - 1){
            struct node *newNode = create_node(b_tree, b_tree->degree, false);
            newNode->child[0] = b_tree->root;
            split_child(newNode, 0, b_tree->root, b_tree);
            int i = 0;
            if (newNode->keys[0] < key) i++;

            insert_node_not_full(b_tree, newNode->child[i], key);
            b_tree->root = newNode;
        }
        else insert_node_not_full(b_tree, b_tree->root, key);
    }
}

void remove_node(struct node *root_node, char key);

int get_pred(struct node *root_node, int idx){
    struct node *curr = root_node->child[idx];
    while (curr->leaf == false) curr = curr->child[curr->numKey];
    return curr->keys[curr->numKey - 1];
}

int get_succ(struct node *root_node, int idx){
    struct node *curr = root_node->child[idx + 1];
    while (curr->leaf==false) curr = curr->child[0];
    return curr->keys[0];
}

void merge(struct node *root_node, int idx){
    struct node *child = root_node->child[idx];
    struct node *sibling = root_node->child[idx + 1];
    child->keys[root_node->degree - 1] = root_node->keys[idx];
    for (int i = 0; i < sibling->numKey; i++) child->keys[i + root_node->degree] = sibling->keys[i];

    if (child->leaf == false){
        for (int i = 0; i <= sibling->numKey; i++) child->child[i + root_node->degree] = sibling->child[i];
    }
    for (int i = idx + 1; i < root_node->numKey; i++) root_node->keys[i - 1] = root_node->keys[i];

    for (int i = idx + 2; i <= root_node->numKey; i++) root_node->child[i - 1] = root_node->child[i];

    child->numKey += sibling->numKey + 1;
    root_node->numKey --;
    free(sibling);
    sibling = NULL;
    return;
}

void remove_nonleaf_node(struct node *root_node, int idx){
    int key = root_node->keys[idx];
    if (root_node->child[idx]->numKey >= root_node->degree){
        int pred = get_pred(root_node, idx);
        root_node->keys[idx] = pred;
        remove_node(root_node->child[idx], pred);
    }
    else if (root_node->child[idx + 1]->numKey >= root_node->degree){
        int succ = get_succ(root_node, idx);
        root_node->keys[idx] = succ;
        remove_node(root_node->child[idx + 1], succ);
    }
    else{
        merge(root_node, idx);
        remove_node(root_node->child[idx], key);
    }
    return;
}

void remove_leaf_node(struct node *root_node, int idx){
    for (int i = idx + 1; i < root_node->numKey; i++) root_node->keys[i - 1] = root_node->keys[i];
    root_node->numKey --;
    return;
}

void take_prev(struct node *root_node, int idx){
    struct node *child = root_node->child[idx];
    struct node *sibling = root_node->child[idx - 1];
    for (int i = child->numKey - 1; i >= 0; i--) child->keys[i + 1] = child->keys[i];
    if (child->leaf==false){
        for (int i = child->numKey; i >= 0; i--) child->child[i + 1] = child->child[i];
    }
    child->keys[0] = root_node->keys[idx - 1];
    if (child->leaf==false) child->child[0] = sibling->child[sibling->numKey];

    root_node->keys[idx - 1] = sibling->keys[sibling->numKey - 1];
    child->numKey++;
    sibling->numKey --;
    return;
}

void take_next(struct node *root_node, int idx){
    struct node *child = root_node->child[idx];
    struct node *sibling = root_node->child[idx + 1];
    child->keys[(child->numKey)] = root_node->keys[idx];
    if (child->leaf == false) child->child[(child->numKey) + 1] = sibling->child[0];

    root_node->keys[idx] = sibling->keys[0];
    for (int i = 1; i < sibling->numKey; i++) sibling->keys[i - 1] = sibling->keys[i];
    if (sibling->leaf == false){
        for (int i = 1; i <= sibling->numKey; i++) sibling->child[i - 1] = sibling->child[i];
    }
    child->numKey++;sibling->numKey --;
    return;
}

void Nodefill(struct node *root_node, int idx){
    if (idx != 0 && root_node->child[idx - 1]->numKey >= root_node->degree) take_prev(root_node, idx);

    else if (idx != root_node->numKey && root_node->child[idx + 1]->numKey >= root_node->degree) take_next(root_node, idx);

    else{
        if (idx != root_node->numKey) merge(root_node, idx);
        else merge(root_node, idx - 1);
    }
    return;
}

int key_search(struct node *root_node, char key){
    int idx = 0;
    while (idx < root_node->numKey && root_node->keys[idx] < key) idx++;
    return idx;
}

void remove_node(struct node *root_node, char key){
    int idx = key_search(root_node, key);
    if (idx < root_node->numKey && root_node->keys[idx] == key){
        if (root_node->leaf ==  true) remove_leaf_node(root_node, idx);
        else remove_nonleaf_node(root_node, idx);
    }else{
    	bool flag;
        if (root_node->leaf == true){
            printf("%c does not exist in the tree\n", key);
            return;
        }
        if(idx == root_node->numKey) flag = true;
		else flag = false;
		
        if (root_node->child[idx]->numKey < root_node->degree) Nodefill(root_node, idx);

        if (flag == true && idx > root_node->numKey) remove_node(root_node->child[idx - 1], key);
        else remove_node(root_node->child[idx], key);
    }
    return;
}

void remove_b_tree(struct root_node *b_tree, char key){
    if (b_tree->root == NULL){
        printf("There is no data\n");
        return;
    }
    remove_node(b_tree->root, key);
    if (b_tree->root->numKey == 0){
        struct node *temp = b_tree->root;
        if (b_tree->root->leaf ==  true) b_tree->root = NULL;
        else b_tree->root = b_tree->root->child[0];
        free(temp);
        temp = NULL;
    }
    return;
}

void print_b_tree(struct node *root_node){
    int i;
    for (i = 0; i < root_node->numKey; i++){
        if (root_node->leaf == false) print_b_tree(root_node->child[i]);
        printf("%c ", root_node->keys[i]);
    }
    if (root_node->leaf == false) print_b_tree(root_node->child[i]);
}

void BTreeCheck(struct root_node *root){
    if (root->root != NULL) print_b_tree(root->root);
}

int main(){
	int menu;
	char data;
	
	struct root_node *root=construct_b_tree(2);
	
	insert_node(root, 'A');insert_node(root, 'C');insert_node(root, 'D');insert_node(root, 'K');insert_node(root, 'M');
	insert_node(root, 'O');insert_node(root, 'P');insert_node(root, 'R');insert_node(root, 'S');insert_node(root, 'T');insert_node(root, 'U');
	while(1){
		printf("1. Insert\n");
		printf("2. Delete\n");
		printf("3. Print\n");
		printf("4. Exit\n");
		printf(">> ");
		scanf("%d",&menu);
		switch(menu){
			case 1:
				insert_node(root, 'E');
			    insert_node(root, 'I');
			    insert_node(root, 'L');
			    insert_node(root, 'G');
			    insert_node(root, 'X');
			    printf("[E,I,L,G,X] Inserted \n");
			    system("pause");
			    system("cls");
			    break;
			case 2:
				printf("Insert data to delete: ");
				scanf("%c",&data);
				data = getchar();
				remove_b_tree(root,data);
				system("pause");
				system("cls");
				break;
			case 3:
				BTreeCheck(root);
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 4:
                exit(0);
				return 0;
		}
	}
    return 0;
}


