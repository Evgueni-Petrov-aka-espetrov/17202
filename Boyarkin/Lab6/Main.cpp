#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>

struct binary_tree {
	int val, h;
	binary_tree *left, *right;
};

binary_tree* create_tree(int val);
void clear_tree(binary_tree *tree);
int get_height(binary_tree *tree);
int get_height(binary_tree *tree);
int balance_factor(binary_tree *tree);
binary_tree* add_branch(binary_tree *tree, int val);
binary_tree* rotate_right(binary_tree *tree);
binary_tree* rotate_left(binary_tree *tree);
binary_tree* balance(binary_tree *tree);

int main() {

	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	binary_tree *tree = NULL;
	int n, val;

	fscanf(in, "%d", &n);

	if (n > 0) {
		fscanf(in, "%d", &val);
		tree = create_tree(val);

		for (int i = 1; i < n; ++i) {
			fscanf(in, "%d", &val);
			tree = add_branch(tree, val);
		}
	}

	fprintf(out, "%d", get_height(tree));

	clear_tree(tree);
	fclose(in);
	fclose(out);
	return 0;
}

binary_tree* create_tree(int val) {
	binary_tree *tree = (binary_tree*)malloc(sizeof(binary_tree));

	tree->val = val;
	tree->h = 1;
	tree->left = tree->right = NULL;

	return tree;
}

void clear_tree(binary_tree *tree) {
	if (tree == NULL) { return; }

	clear_tree(tree->left);
	clear_tree(tree->right);
	free(tree);
}

int get_height(binary_tree *tree) {
	return (tree != NULL) ? tree->h : 0;
}

void fix_height(binary_tree *tree) {
	tree->h = __max(get_height(tree->left), get_height(tree->right)) + 1;
}

int balance_factor(binary_tree *tree) {
	return get_height(tree->right) - get_height(tree->left);
}

binary_tree* add_branch(binary_tree *tree, int val) {
	if (tree == NULL) { return create_tree(val); }

	if (val < tree->val) { tree->left = add_branch(tree->left, val); }
	else { tree->right = add_branch(tree->right, val); }

	return balance(tree);
}

binary_tree* rotate_right(binary_tree *tree) {
	binary_tree *temp = tree->left;
	tree->left = temp->right;
	temp->right = tree;

	fix_height(tree);
	fix_height(temp);
	return temp;
}

binary_tree* rotate_left(binary_tree *tree) {
	binary_tree *temp = tree->right;
	tree->right = temp->left;
	temp->left = tree;

	fix_height(tree);
	fix_height(temp);
	return temp;
}

binary_tree* balance(binary_tree *tree) {
	fix_height(tree);

	if (balance_factor(tree) == 2) {
		if (balance_factor(tree->right) < 0) {
			tree->right = rotate_right(tree->right);
		}
		return rotate_left(tree);
	}
	if (balance_factor(tree) == -2) {
		if (balance_factor(tree->left) > 0) {
			tree->left = rotate_left(tree->left);
		}
		return rotate_right(tree);
	}
	return tree;
}
