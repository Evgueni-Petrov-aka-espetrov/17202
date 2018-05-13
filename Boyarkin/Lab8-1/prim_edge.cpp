#include "prim_edge.h"

prim_edge **create_graph(edge *edges, int edges_count, int vertex_count) {
	prim_edge **graph = (prim_edge**)malloc(sizeof(prim_edge*) * vertex_count);

	int *freq = get_frequency(edges, edges_count, vertex_count);
	for (int i = 0; i < vertex_count; ++i) {
		graph[i] = (prim_edge*)malloc(sizeof(prim_edge) * freq[i]);
	}

	memset(freq, 0, sizeof(int) * vertex_count);
	for (int i = 0; i < edges_count; ++i) {
		graph[edges[i].L][freq[edges[i].L]].vertex = edges[i].R;
		graph[edges[i].L][freq[edges[i].L]].edge_num = i;
		graph[edges[i].L][freq[edges[i].L]].weight = edges[i].weight;
		++freq[edges[i].L];

		graph[edges[i].R][freq[edges[i].R]].vertex = edges[i].L;
		graph[edges[i].R][freq[edges[i].R]].edge_num = i;
		graph[edges[i].R][freq[edges[i].R]].weight = edges[i].weight;
		++freq[edges[i].R];
	}

	free(freq);
	return graph;
}

void free_graph(prim_edge **graph, int vertex_count) {
	for (int i = 0; i < vertex_count; ++i) {
		free(graph[i]);
	}
	free(graph);
}

int* get_frequency(edge *edges, int edges_count, int vertex_count) {
	int *freq = (int*)calloc(vertex_count, sizeof(int));

	for (int i = 0; i < edges_count; ++i) {
		++freq[edges[i].L];
		++freq[edges[i].R];
	}

	return freq;
}
