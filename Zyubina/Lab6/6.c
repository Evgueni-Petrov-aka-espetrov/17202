#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct tree
{
	int value;
	unsigned char height;
	struct tree* left;
	struct tree* right;
};

unsigned char height(struct tree* tree){
	if (tree != NULL)
		return tree->height;
	else
		return 0;
}

void calc_height(struct tree *tree) {
	if (height(tree->left) > height(tree->right))
		tree->height = height(tree->left) + 1;
	else
		tree->height = height(tree->right) + 1;
}

struct tree* left_rot(struct tree* tree) {
	struct tree* node = tree->right;
	tree->right = node->left;
	node->left = tree;
	calc_height(tree);
	calc_height(node);
	return node;
}

struct tree* right_rot(struct tree* tree) {
	struct tree* node = tree->left;
	tree->left = node->right;
	node->right = tree;
	calc_height(tree);
	calc_height(node);
	return node;
}

struct tree* balance(struct tree* tree) {
	int a = height(tree->right) - height(tree->left);
	if (a == 2) {
		if (height(tree->right->left) > height(tree->right->right))
			tree->right = right_rot(tree->right);
		return left_rot(tree);
	}
	if (a == -2) {
		if (height(tree->left->right) > height(tree->left->left))
			tree->left = left_rot(tree->left);
		return right_rot(tree);
	}
	return tree;
}

struct tree* create_tree(struct tree* tree, int val) {
	tree = (struct tree*)malloc(sizeof(struct tree));
	tree->value = val;
	tree->height = 1;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}

struct tree* insert(struct tree* tree, int val) {;
	if (!tree)
		return create_tree(tree, val);
	if (val > tree->value)
		tree->right = insert(tree->right, val);
	else
		tree->left = insert(tree->left, val);
	calc_height(tree);
	return balance(tree);
}

struct tree* free_tree(struct tree* tree) {
	if (tree != NULL) {
		free_tree(tree->left);
		free_tree(tree->right);
		free(tree);
	}
	return NULL;
}

int main() {
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");

	int N;
	fscanf(in, "%d", &N);
	struct tree* tree = NULL;
	int val;
	for (int i = 0; i < N; i++) {
		fscanf(in, "%d", &val);
		tree = insert(tree, val);
	}
	fprintf(out, "%d", height(tree));
	free_tree(tree);
	fclose(in);
	fclose(out);
}
