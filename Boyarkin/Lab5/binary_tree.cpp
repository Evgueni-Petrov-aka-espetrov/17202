#include "binary_tree.h"

binary_tree* create_branch() {
	binary_tree *tree = (binary_tree*)malloc(sizeof(binary_tree));
	tree->left = tree->right = NULL;

	return tree;
}

binary_tree* build_tree(char **cipher, int symbol_count) {
	binary_tree *tree = create_branch();
	binary_tree *path;

	for (int i = 0; i < symbol_count; ++i) {
		path = tree;
		for (char *c = cipher[i]; *c != '\0'; ++c) {
			if (*c == '0') {
				if (path->left == NULL) { path->left = create_branch(); }
				path = path->left;
			}
			else if (*c == '1') {
				if (path->right == NULL) { path->right = create_branch(); }
				path = path->right;
			}
		}
		path->val = (char)i;
	}
	return tree;
}

void clear_tree(binary_tree *tree) {
	if (tree == NULL) { return; }

	clear_tree(tree->left);
	clear_tree(tree->right);
	free(tree);
}
