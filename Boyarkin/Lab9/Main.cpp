#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable : 4996)

struct list {
	int val;
	struct list *next;
};
typedef struct list List;

const int MAX_VRTX_NUM = 5000;

int** create_edges(int n);
void clear_edges(int **path, int size);
void clear_list(List *ls);

int main() {

	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	int **edges = NULL;
	unsigned int *path = NULL;
	bool *to_visit = NULL;
	bool *visited = NULL;

	int n, m, s, f, lhs, rhs;
	long long length;


	fscanf(in, "%d", &n);
	if ((n < 0) || (n > MAX_VRTX_NUM)) {
		fprintf(out, "bad number of vertices");
		goto END;
	}

	fscanf(in, "%d%d", &s, &f);
	--s;
	--f;
	if ((s < 0) || (s >= n) || (f < 0) || (f >= n)) {
		fprintf(out, "bad vertex");
		goto END;
	}

	fscanf(in, "%d", &m);
	if ((m < 0) || (m >((n * (n + 1)) / 2))) {
		fprintf(out, "bad number of edges");
		goto END;
	}

	edges = create_edges(n);
	path = (unsigned int*)malloc(sizeof(unsigned int)* n);
	for (int i = 0; i < n; ++i) { path[i] = INT_MAX + 1u; }
	to_visit = (bool*)malloc(sizeof(bool)* n);
	memset(to_visit, 0, sizeof(bool)* n);
	visited = (bool*)malloc(sizeof(bool)* n);
	memset(visited, 0, sizeof(bool)* n);

	for (int i = 0; i < m; ++i) {
		if (feof(in)) {
			fprintf(out, "bad number of lines");
			goto END;
		}

		fscanf(in, "%d%d", &lhs, &rhs);
		--lhs;
		--rhs;
		if ((lhs < 0) || (lhs >= n) || (rhs < 0) || (rhs >= n)) {
			fprintf(out, "bad vertex");
			goto END;
		}

		fscanf(in, "%lld", &length);
		if ((length < 0) || (length > INT_MAX)) {
			fprintf(out, "bad length");
			goto END;
		}

		edges[lhs][rhs] = edges[rhs][lhs] = (int)length;
	}

	to_visit[s] = true;
	path[s] = 0;

	unsigned int min_path;
	int min_id;
	while (true) {
		min_id = -1;

		for (int i = 0; i < n; ++i) {
			if (to_visit[i]) {
				min_id = i;
				break;
			}
		}
		if (min_id == -1) { break; }

		for (int i = min_id + 1; i < n; ++i) {
			if (to_visit[i] && (path[i] < path[min_id])) {
				min_id = i;
			}
		}

		for (int i = 0; i < n; ++i) {
			if (!visited[i] && edges[min_id][i] != -1) {
				to_visit[i] = true;
			}
			if (path[min_id] + edges[min_id][i] >= edges[min_id][i]) {

				path[i] = __min(path[min_id] + edges[min_id][i], path[i]);
				if (path[i] > INT_MAX) { path[i] = INT_MAX + 1u; }
			}
		}

		to_visit[min_id] = false;
		visited[min_id] = true;
	}

	// Output:

	for (int i = 0; i < n; ++i) {
		if (!visited[i]) { fprintf(out, "oo"); }
		else if (path[i] > INT_MAX) { fprintf(out, "INT_MAX+"); }
		else { fprintf(out, "%d", path[i]); }
		fprintf(out, " ");
	}
	fprintf(out, "\n");

	if (!visited[f]) { fprintf(out, "no path"); }
	else {
		// Overflow check

		List *ls_path = (List*)malloc(sizeof(List));
		List *ls_end = ls_path;
		int next_pos;

		memset(visited, 0, sizeof(bool)* n);

		visited[f] = true;
		ls_path->val = f + 1;
		ls_path->next = NULL;
		while (path[f] > INT_MAX) {
			next_pos = -1;

			for (int i = 0; i < n; ++i) {
				if (!visited[i] && edges[f][i] != -1) {

					if (next_pos == -1) { next_pos = i; }
					else {
						fprintf(out, "overflow");
						clear_list(ls_path);
						goto END;
					}
				}
			}

			f = next_pos;
			visited[next_pos] = true;

			ls_end->next = (List*)malloc(sizeof(List));
			ls_end = ls_end->next;
			ls_end->val = f + 1;
			ls_end->next = NULL;
		}

		for (List *iter = ls_path; iter != NULL; iter = iter->next) {
			fprintf(out, "%d ", iter->val);
		}
		clear_list(ls_path);

		// After Overflow:

		while (f != s) {
			for (int i = 0; i < n; ++i) {
				if (visited[i]) { continue; }

				if ((path[f] - path[i]) == edges[f][i]) {
					f = i;

					fprintf(out, "%d ", f + 1);
					visited[f] = true;

					break;
				}
			}
		}
	}

END:
	clear_edges(edges, n);
	if (path) { free(path); }
	if (visited) { free(visited); }

	fclose(in);
	fclose(out);
	return 0;
}

int** create_edges(int n) {
	int **path;

	path = (int**)malloc(sizeof(int*)* n);
	for (int i = 0; i < n; ++i) {
		path[i] = (int*)malloc(sizeof(int)* n);

		for (int j = 0; j < n; ++j) {
			path[i][j] = -1;
		}
		path[i][i] = 0;
	}

	return path;
}

void clear_edges(int **edges, int size) {
	if (edges == NULL) { return; }

	for (int i = 0; i < size; ++i) {
		free(edges[i]);
	}
	free(edges);
}

void clear_list(List *ls) {
	List *iter = ls;
	List *iter_next;

	while (iter != NULL) {
		iter_next = iter->next;
		free(iter);
		iter = iter_next;
	}
}
