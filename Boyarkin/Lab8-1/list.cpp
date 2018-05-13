#include "list.h"

list *create_list(edge *val) {
	list *ls = (list*)malloc(sizeof(list));
	ls->val = val;
	ls->next = NULL;

	return ls;
}

void add_list(list **ls, edge *e) {
	list *obj = create_list(e);

	if (*ls == NULL) {
		*ls = obj;
		return;
	}
	else if (obj->val->weight <= (*ls)->val->weight) {
		obj->next = *ls;
		*ls = obj;
	}

	list *prev = *ls;
	list *iter = prev->next;

	for (; iter != NULL; prev = iter, iter = iter->next) {
		if (obj->val->weight <= iter->val->weight) { break; }
	}

	prev->next = obj;
	obj->next = iter;
}

void pop_list(list **ls) {
	if (*ls == NULL) { return; }

	list *temp = (*ls)->next;
	free(*ls);
	*ls = temp;
}
