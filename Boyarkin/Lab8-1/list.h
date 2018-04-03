#pragma once
#include <stdlib.h>
#include "edge.h"

struct list {
	edge *val;
	list *next;
};

list *create_list(edge *val);
void add_list(list **ls, edge *e);
void pop_list(list **ls);
