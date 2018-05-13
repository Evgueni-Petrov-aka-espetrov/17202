#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include "edge.h"
#include "list.h"
#include "prim_edge.h"

const int MAX_VRTX_NUM = 5000;

bool* prim(edge *edges, int edges_count, int vertex_count);
prim_edge **create_graph(edge *edges, int edges_count, int vertex_count);
void free_graph(prim_edge **graph, int vertex_count);
int* get_frequency(edge *edges, int edges_count, int vertex_count);
void update_queue(list **ls, const int *min_path);

int main() {
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	edge *edges = NULL;
	int n, m;

	fscanf(in, "%d", &n);
	if ((n < 0) || (n > MAX_VRTX_NUM)) {
		fprintf(out, "bad number of vertices");
		goto END;
	}

	fscanf(in, "%d", &m);
	if ((m < 0) || (m >((n * (n + 1)) / 2))) {
		fprintf(out, "bad number of edges");
		goto END;
	}

	edges = (edge*)malloc(m * sizeof(edge));

	int scan_res;
	for (int i = 0; i < m; ++i) {
		scan_res = fscanf(in, "%d%d%d", &(edges[i].L), &(edges[i].R), &(edges[i].weight));
		if (scan_res < 3) {
			fprintf(out, "bad number of lines");
			goto END;
		}

		--edges[i].L;
		--edges[i].R;
		if ((edges[i].L < 0) || (edges[i].L >= n) || (edges[i].R < 0) || (edges[i].R >= n)) {
			fprintf(out, "bad vertex");
			goto END;
		}

		if (edges[i].weight < 0) {
			fprintf(out, "bad length");
			goto END;
		}
		--edges[i].weight;
	}

	if (n == 0) {
		fprintf(out, "no spanning tree");
		goto END;
	}
	if (n == 1) { goto END; }

	bool *used_edges = prim(edges, m, n);

	if (used_edges != NULL) {
		for (int i = 0; i < m; ++i) {
			if (used_edges[i]) {
				fprintf(out, "%d %d\n", edges[i].L + 1, edges[i].R + 1);
			}
		}
		free(used_edges);
	}
	else {
		fprintf(out, "no spanning tree");
	}

END:
	free(edges);
	fclose(in);
	fclose(out);
	return 0;
}

bool* prim(edge *edges, int edges_count, int vertex_count) {
	int *freq = get_frequency(edges, edges_count, vertex_count);
	prim_edge **graph = create_graph(edges, edges_count, vertex_count);
	bool *used_edges = (bool*)calloc(edges_count, sizeof(bool));
	int *min_path = (int*)malloc(vertex_count * sizeof(int));
	edge *current_edge;
	prim_edge next_prim;
	list *queue = NULL;
	int edge_id, current_vert;

	for (int i = 0; i < vertex_count; ++i) { min_path[i] = INT_MAX; }
	
	min_path[0] = INT_MIN;
	for (int i = 0; i < freq[0]; ++i) {
		add_list(&queue, &(edges[graph[0][i].edge_num]));
	}

	while (queue != NULL) {
		current_edge = queue->val;
		pop_list(&queue);

		edge_id = (current_edge - edges);
		used_edges[edge_id] = true;
		
		if (min_path[current_edge->L] != INT_MIN) {
			current_vert = current_edge->L;
		}
		else {
			current_vert = current_edge->R;
		}

		min_path[current_vert] = INT_MIN;

		for (int i = 0; i < freq[current_vert]; ++i) {
			next_prim = graph[current_vert][i];

			if (next_prim.weight < min_path[next_prim.vertex]) {
				min_path[next_prim.vertex] = next_prim.weight;
				add_list(&queue, edges + next_prim.edge_num);
			}
		}

		update_queue(&queue, min_path);
	}

	for (int i = 0; i < vertex_count; ++i) {
		if (min_path[i] != INT_MIN) {
			free(used_edges);
			used_edges = NULL;
			break;
		}
	}
	
	free_graph(graph, vertex_count);
	free(freq);
	free(min_path);
	return used_edges;
}

void update_queue(list **ls, const int *min_path) {
	int L, R, weight;

	while ((*ls) != NULL) {
		L = (*ls)->val->L;
		R = (*ls)->val->R;
		weight = (*ls)->val->weight;

		if ((min_path[L] < weight) && (min_path[R] < weight)) {
			pop_list(ls);
		}
		else { break; }
	}
	if ((*ls) == NULL) { return; }

	list *prev = *ls;
	list *curr = (*ls)->next;
	list *temp;
	while (curr != NULL) {
		L = curr->val->L;
		R = curr->val->R;
		weight = curr->val->weight;

		if ((min_path[L] < weight) && (min_path[R] < weight)) {
			temp = curr;
			curr = curr->next;
			prev->next = curr;
			free(temp);
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
}
