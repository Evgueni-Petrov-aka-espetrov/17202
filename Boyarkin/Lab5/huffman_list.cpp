#include "huffman_list.h"

huffman_list* create_list(int symbols_count) {
	huffman_list *list = (huffman_list*)malloc(sizeof(huffman_list));
	list->symbols = (unsigned char*)malloc(sizeof(unsigned char) * symbols_count);
	list->len = 0;
	list->next = NULL;

	return list;
}

void add_list(huffman_list **list, huffman_list *obj) {
	if (*list == NULL) {
		*list = obj;
		return;
	}
	else if (obj->val <= (*list)->val) {
		obj->next = *list;
		*list = obj;
	}

	huffman_list *prev = *list;
	huffman_list *iter = prev->next;

	for (; iter != NULL; prev = iter, iter = iter->next) {
		if (obj->val <= iter->val) { break; }
	}

	prev->next = obj;
	obj->next = iter;
}

void pop_list(huffman_list **list) {
	if (*list == NULL) { return; }

	huffman_list *temp = (*list)->next;
	free((*list)->symbols);
	free(*list);
	*list = temp;
}

void combine_symbols(huffman_list *dest, huffman_list *lhs, huffman_list *rhs) {
	dest->len = 0;
	for (int i = 0; i < lhs->len; ++i) {
		dest->symbols[dest->len] = lhs->symbols[i];
		++dest->len;
	}
	for (int i = 0; i < rhs->len; ++i) {
		dest->symbols[dest->len] = rhs->symbols[i];
		++dest->len;
	}
}

void clear_list(huffman_list **list) {
	while (*list != NULL) {
		pop_list(list);
	}
}
