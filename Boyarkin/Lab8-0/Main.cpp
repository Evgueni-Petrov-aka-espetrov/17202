#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_VRTX_NUM = 5000;

struct edge {
	int L, R;
	int weight;
};

int edge_comparer(const void *lhs, const void *rhs);
bool* kruskal(edge *edges, int edges_count, int vertex_count);

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
	if ((m < 0) || (m > ((n * (n + 1)) / 2))) {
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
	}
	
	qsort(edges, m, sizeof(edge), edge_comparer);

	if (n == 0) {
		fprintf(out, "no spanning tree");
		goto END;
	}
	if (n == 1) { goto END; }
	bool *used_edges = kruskal(edges, m, n);
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

int edge_comparer(const void *lhs, const void *rhs) {
	return ((edge*)lhs)->weight - ((edge*)rhs)->weight;
}

bool* kruskal(edge *edges, int edges_count, int vertex_count) {
	bool *used_edges = (bool*)calloc(edges_count, sizeof(bool));
	int *color = (int*)calloc(vertex_count, sizeof(int));
	int *vert_left, *vert_right;
	int color_from, color_to;
	int next_color = 1;
	
	for (int i = 0; i < edges_count; ++i) {
		vert_left = color + edges[i].L;
		vert_right = color + edges[i].R;

		if (!(*vert_left) && !(*vert_right)) {
			*vert_left = *vert_right = next_color++;
			used_edges[i] = true;
		}
		else if (!(*vert_left)) {
			*vert_left = *vert_right;
			used_edges[i] = true;
		}
		else if (!(*vert_right)) {
			*vert_right = *vert_left;
			used_edges[i] = true;
		}
		else if (*vert_left != *vert_right) {
			color_from = __max(*vert_left, *vert_right);
			color_to = __min(*vert_left, *vert_right);

			for (int j = 0; j < vertex_count; ++j) {
				if (color[j] == color_from) {
					color[j] = color_to;
				}
			}
			used_edges[i] = true;
		}
	}

	for (int i = 0; i < vertex_count; ++i) {
		if (color[i] != 1) {
			free(used_edges);
			used_edges = NULL;
			break;
		}
	}

	free(color);
	return used_edges;
}
