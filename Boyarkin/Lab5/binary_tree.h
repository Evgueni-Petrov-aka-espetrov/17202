#pragma once
#include <stdlib.h>

struct binary_tree {
	char val;
	binary_tree *left, *right;
};

binary_tree* create_branch();
binary_tree* build_tree(char **cipher, int symbol_count);
void clear_tree(binary_tree *tree);
