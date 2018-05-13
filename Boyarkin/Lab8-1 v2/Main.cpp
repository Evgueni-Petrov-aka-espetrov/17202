#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>

const int MAX_VRTX_NUM = 5000;

struct pair {
	int left, right;
};

int **create_edges(int vertex_count);
void free_edges(int **edges, int vertex_count);
pair* prim(int **edges, int vertex_count);

int main() {
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	pair *result;
	int **edges = NULL;
	int edge_left, edge_right, weight;
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

	edges = create_edges(n);

	int scan_res;
	for (int i = 0; i < m; ++i) {
		scan_res = fscanf(in, "%d%d%d", &edge_left, &edge_right, &weight);
		if (scan_res < 3) {
			fprintf(out, "bad number of lines");
			goto END;
		}

		--edge_left;
		--edge_right;
		if ((edge_left < 0) || (edge_left >= n) || (edge_right < 0) || (edge_right >= n)) {
			fprintf(out, "bad vertex");
			goto END;
		}

		if (weight < 0) {
			fprintf(out, "bad length");
			goto END;
		}
		--weight;

		edges[edge_left][edge_right] = edges[edge_right][edge_left] = weight;
	}

	if (n == 0) {
		fprintf(out, "no spanning tree");
		goto END;
	}
	if (n == 1) { goto END; }

	result = prim(edges, n);

	if (result != NULL) {
		for (int i = 0; i < (n - 1); ++i) {
			fprintf(out, "%d %d\n", result[i].left + 1, result[i].right + 1);
		}
		free(result);
	}
	else {
		fprintf(out, "no spanning tree");
	}

END:
	free_edges(edges, n);
	fclose(in);
	fclose(out);
	return 0;
}

int **create_edges(int vertex_count) {
	int **edges = (int**)malloc(sizeof(int*) * vertex_count);

	for (int i = 0; i < vertex_count; ++i) {
		edges[i] = (int*)malloc(sizeof(int) * vertex_count);
		for (int j = 0; j < vertex_count; ++j) {
			edges[i][j] = INT_MAX;
		}
	}

	return edges;
}

void free_edges(int **edges, int vertex_count) {
	if (edges == NULL) { return; }

	for (int i = 0; i < vertex_count; ++i) {
		free(edges[i]);
	}
	free(edges);
}

pair* prim(int **edges, int vertex_count) {
	pair *result = (pair*)malloc(sizeof(pair) * (vertex_count - 1));
	pair *min_path = (pair*)malloc(sizeof(pair) * vertex_count);
	int next;

	//min_path members: left - "min path"; right - "vertex from".

	for (int i = 0; i < vertex_count; ++i) { min_path[i].left = INT_MAX; }

	min_path[0].left = INT_MIN;
	for (int i = 1; i < vertex_count; ++i) {
		if (edges[0][i] != INT_MAX) {
			min_path[i].left = edges[0][i];
			min_path[i].right = 0;
		}
	}

	for (int i = 0; i < (vertex_count - 1); ++i) {
		next = -1;

		for (int j = 0; j < vertex_count; ++j) {
			// If "min" - already visited. If "max" - unrachable yet
			if ((min_path[j].left == INT_MIN) || (min_path[j].left == INT_MAX)) { continue; }

			if ((next == -1) || (min_path[j].left < min_path[next].left)) {
				next = j;
			}
		}

		if (next != -1) {
			result[i].left = next;
			result[i].right = min_path[next].right;
		}
		else {
			// Not all vertexes were visited
			free(result);
			result = NULL;
			break;
		}

		min_path[next].left = INT_MIN;
		for (int j = 0; j < vertex_count; ++j) {
			if ((edges[next][j] == INT_MAX) || (min_path[j].left == INT_MIN)) { continue; }

			if (edges[next][j] < min_path[j].left) {
				min_path[j].left = edges[next][j];
				min_path[j].right = next;
			}
		}
	}

	free(min_path);
	return result;
}
