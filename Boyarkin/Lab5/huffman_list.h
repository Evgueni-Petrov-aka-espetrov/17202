#pragma once
#include <stdlib.h>

struct huffman_list {
	int val, len;
	unsigned char *symbols;

	huffman_list *next;
};

huffman_list* create_list(int symbols_count);
void add_list(huffman_list **list, huffman_list *obj);
void pop_list(huffman_list **list);
void combine_symbols(huffman_list *dest, huffman_list *lhs, huffman_list *rhs);
void clear_list(huffman_list **list);
