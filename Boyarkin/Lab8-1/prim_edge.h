#pragma once
#include <stdlib.h>
#include <string.h>
#include "edge.h"

struct prim_edge {
	int vertex;
	int weight;
	int edge_num;
};

prim_edge **create_graph(edge *edges, int edges_count, int vertex_count);
void free_graph(prim_edge **graph, int vertex_count);
int* get_frequency(edge *edges, int edges_count, int vertex_count);
