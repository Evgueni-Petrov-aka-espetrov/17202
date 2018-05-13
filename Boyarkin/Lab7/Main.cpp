#pragma warning (disable : 4996)
#include <stdio.h>
#include <stdlib.h>

void swap(int *lhs, int *rhs);
bool** get_graph(FILE *in, int n, int m);
bool **create_graph(int n);
void free_graph(bool **edges, int n);
int* sort_graph(bool **edges, int n);

int error = 0;

int main() {

	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	bool **edges = NULL;
	int *order = NULL;
	int n = 0, m = 0;

	if (fscanf(in, "%d", &n) < 1) {
		fprintf(out, "bad number of lines");
		goto END;
	}
	if ((n < 0) || (n > 1000)) {
		fprintf(out, "bad number of vertices");
		goto END;
	}

	if (fscanf(in, "%d", &m) < 1) {
		fprintf(out, "bad number of lines");
		goto END;
	}
	if ((m < 0) || (m > ((n * (n + 1)) / 2))) {
		fprintf(out, "bad number of edges");
		goto END;
	}

	// —читываем граф
	edges = get_graph(in, n, m);
	if (error) {
		if (error == 1) { fprintf(out, "bad number of lines"); }
		if (error == 2) { fprintf(out, "bad vertex"); }
		goto END;
	}

	// —ортируем граф и выводим результат
	order = sort_graph(edges, n);
	if (order != NULL) {
		for (int i = 0; i < n; ++i) {
			fprintf(out, "%d ", order[i] + 1);
		}
	}
	else {
		fprintf(out, "impossible to sort");
	}

END:
	free_graph(edges, n);
	free(order);
	fclose(in);
	fclose(out);
	return 0;
}

void swap(int *lhs, int *rhs) {
	*lhs ^= *rhs;
	*rhs ^= *lhs;
	*lhs ^= *rhs;
}

bool** get_graph(FILE *in, int n, int m) {
	bool **edges = create_graph(n);
	int lhs, rhs;

	for (int i = 0; i < m; ++i) {
		if (fscanf(in, "%d%d", &lhs, &rhs) < 2) {
			error = 1;
			return edges;
		}
		--lhs; --rhs;

		if ((lhs < 0) || (lhs >= n) || (rhs < 0) || (rhs >= n)) {
			error = 2;
			return edges;
		}

		edges[lhs][rhs] = true;
	}

	return edges;
}

bool **create_graph(int n) {
	bool **edges;

	edges = (bool**)malloc(sizeof(bool*) * n);
	for (int i = 0; i < n; ++i) {
		edges[i] = (bool*)calloc(n, sizeof(bool));
	}

	return edges;
}

void free_graph(bool **edges, int n) {
	if (edges == NULL) { return; }

	for (int i = 0; i < n; ++i) { free(edges[i]); }
	free(edges);
}

int* sort_graph(bool **edges, int n) {
	int *order = (int*)calloc(n, sizeof(int));
	for (int i = 0; i < n; ++i) { order[i] = i; }

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (edges[order[j]][order[i]]) { swap(&order[i], &order[j]); }
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = i; j < n; ++j) {
			if (edges[order[j]][order[i]]) {
				free(order);
				return NULL;
			}
		}
	}

	return order;
}
