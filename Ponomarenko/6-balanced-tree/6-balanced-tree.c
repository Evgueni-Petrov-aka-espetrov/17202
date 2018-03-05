#include <stdio.h>
#include <malloc.h>
typedef struct TNode {
	int value;
	int height;
	struct TNode* left;
	struct TNode* right;
} Node;

typedef struct TTree {
	Node* root;
} Tree;
int max(int a, int b) 
{
	return (a > b) ? a : b;
}

int avl_height(Node* node) {
	if (node == NULL) {
		return 0;
	}
	else {
		return node->height;
	}
}

void avl_print_node(Node* node) {
	if (node == NULL) {
		return;
	}
	avl_print_node(node->left);
	printf("%d ", node->value);
	avl_print_node(node->right);
	return;
}

void avl_print_tree(Tree* tree) {
	avl_print_node(tree->root);
	printf("\n");
}

Tree* avl_create_tree()
{
	Tree* node = malloc(sizeof(Tree));
	if (node == NULL) {
		printf("Not enough memory");
		exit(0);
	}
	node->root = NULL;
	return node;
}

void avl_fix_height(Node* node)
{	
	if (node != NULL)
	{
		node->height = max(avl_height(node->left), avl_height(node->right)) + 1;
	}
}

void avl_rturn(Node** root) {

	Node*  ptr_help = (*root)->left;
	(*root)->left = (*root)->left->right;
	ptr_help->right = *root;
	*root = ptr_help;

	avl_fix_height((*root)->right);
	avl_fix_height(*root);
}

void avl_lturn(Node** root)
{

	Node*  ptr_help = (*root)->right;
	(*root)->right = (*root)->right->left;
	ptr_help->left = *root;
	*root = ptr_help;

	avl_fix_height((*root)->left);
	avl_fix_height(*root);

}

void avl_balance(Node** root)
{
	if ((avl_height((*root)->left) - avl_height((*root)->right)) >= 2) {
		if (avl_height((*root)->left->left) < avl_height((*root)->left->right)) {
			avl_lturn(&((*root)->left));
		}
		avl_rturn(root);
	}
	else if ((avl_height((*root)->right) - avl_height((*root)->left)) >= 2) {
		if (avl_height((*root)->right->left) > avl_height((*root)->right->right)) {
			avl_rturn(&((*root)->right));
		}
		avl_lturn(root);
	}
}

Node* avl_create_node(int value)
{
	Node* node;
	node = malloc(sizeof(Node));
	if (node == NULL) {
		exit(0);
	}
	
	node->value = value;
	node->height = 1;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void avl_insert_node(Node** node, int value)
{

	if (*node == NULL) {
		*node = avl_create_node(value);

	}
	else {
		if (value <= (*node)->value) {
			avl_insert_node(&((*node)->left), value);
			
		}
		else {
			avl_insert_node(&((*node)->right), value);
		}
		avl_balance(node);
		avl_fix_height(*node);
	}
	return;
}


void avl_insert_tree(Tree* tree, int value)
{
	avl_insert_node(&(tree->root), value);
	
}

void avl_free_node(Node* node) {
	if (node == NULL) {
		return;
	}
	avl_free_node(node->left);
	avl_free_node(node->right);
	free(node);
	return;
}


void avl_free_tree(Tree* tree) {
	avl_free_node(tree->root);
	free(tree);
	return;
}


int main(){
	Tree* first = avl_create_tree();
	int val;
	int num;
	scanf("%d", &num);

	for (int i = 0; i < num; ++i) {
		scanf("%d", &val);
		avl_insert_tree(first, val);
	}

	printf(" %d ", avl_height(first->root));
	avl_free_tree(first);
	return 0;
}
