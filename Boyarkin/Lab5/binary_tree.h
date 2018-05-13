#pragma once
#include <stdlib.h>

struct binary_tree {
	char val;
	binary_tree *left, *right;
};

binary_tree* create_branch();
binary_tree* build_huffman_tree(char **cipher);
void clear_tree(binary_tree *tree);
