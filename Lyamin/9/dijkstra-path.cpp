#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

enum { INF = -1 };

struct vertex {
	unsigned long long distance;
	int prev_ver;
	int used;
	int only_way;
};

vertex* vertex_ctor(int N) {
	vertex* result = (vertex*)malloc(sizeof(vertex)*N);
	for (int i = 0; i < N; ++i) {
		result[i].distance = INF;
		result[i].used = 0;
		result[i].prev_ver = -1;
	}
	return result;
}

void vertex_and_table_dtor(vertex* in, int** table, int N) {
	if (in) free(in);
	for (int i = 0; i < N; ++i) {
		free(table[i]);
	}
	free(table);
	return;
}

int valid(FILE* output, int N, int M, int S, int F) {
	if (N > 5000 || N < 0) { fprintf(output, "bad number of vertices"); return 0; }
	if (M > N * (N - 1) / 2 || M < 0) { fprintf(output, "bad number of edges");	return 0; }
	if (S > N || S < 1 || F > N || F < 1) { fprintf(output, "bad vertex"); return 0; }
	return 1;
}

int bad_input(FILE* output, int ver1, int ver2, long long length, int N) {

	if (ver1 > N || ver1 < 1 || ver2 > N || ver2 < 1) {
		fprintf(output, "bad vertex");
		return 1;
	}
	if (length < 0) {
		fprintf(output, "bad length");
		return 1;
	}
	else {
		if (length > INT_MAX) {
			fprintf(output, "bad length");
			return 1;
		}
	}
	return 0;
}

int** table_ctor(int N) {
	int** result = (int**)malloc(sizeof(int*)*N);
	for (int i = 0; i < N; ++i) {
		result[i] = (int*)malloc(sizeof(int)*N);
	}
	return result;
}

int** fill_table(FILE* input, FILE* output, int** table, int M, int N) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			table[i][j] = INF;
		}
	}

	for (int i = 0; i < M; ++i) {
		int ver1, ver2;
		long long length;
		fscanf(input, "%d%d", &ver1, &ver2);
		fscanf(input, "%lli", &length);

		if (bad_input(output, ver1, ver2, length, N)) {
			return NULL; 
		}
		--ver1; --ver2;

		table[ver2][ver1] = length;
		table[ver1][ver2] = length;
		if (feof(input) && i < M - 1) {
			fprintf(output, "bad number of lines");

			return NULL;
		}
	}

	for (int i = 0; i < N; ++i)
		table[i][i] = 0;

	return table;
}

void print_all_distance(FILE* output, vertex* vertexes, int N) {
	for (int i = 0; i < N; ++i) {
		long long tmp = vertexes[i].distance;
		
		if (tmp == INF) {
			fprintf(output, "oo ");
			continue;
		}
		if (tmp > INT_MAX) {
			fprintf(output, "INT_MAX+ ");
			continue;
		}
		fprintf(output, "%I64d ", tmp);
	}
	fprintf(output, "\n");
}

void print_path(FILE* output, int** table, vertex* vertexes, int S, int F, int N) {
	if (vertexes[F].distance == INF) {
		fprintf(output, "no path");
		return;
	}
	int result[5001];
	int flag = 0;

	for (int i = F; i != S; i = vertexes[i].prev_ver, ++flag) {
		result[flag] = i + 1;
		if ((vertexes[i].distance > INT_MAX) && (!vertexes[i].only_way)) {
			fprintf(output, "overflow");
			return;
		}
	}

	for (int i = 0; i < flag; ++i) {
		fprintf(output, "%d ", result[i]);
	}

	fprintf(output, "%d", S + 1);
	return;
}

void dijkstra_path(int** table, vertex* vertexes, int S, int F, int N) {
	vertexes[S].distance = 0;
	
	while(true) {
		int v = -1;
		for (unsigned int j = 0; j < N; ++j)
			if ((!vertexes[j].used) && ((v == -1) || (vertexes[j].distance < vertexes[v].distance)))
				v = j;

		if (v == -1) { break; }
		vertexes[v].used = 1;

		for (int k = 0; k < N; ++k) {
			if (k == v) { continue; }
			if (table[v][k] != INF) {	
				if ((vertexes[k].distance == INF) || (vertexes[v].distance + (unsigned long long)table[v][k] < vertexes[k].distance)) {
					vertexes[k].distance = vertexes[v].distance + (unsigned long long)table[v][k];
					vertexes[k].prev_ver = v;
					vertexes[k].only_way = 1;
					continue;
				}

				if ((vertexes[v].distance + (unsigned long long)table[v][k]) == vertexes[k].distance) {
					vertexes[k].only_way = 0;
					continue;
				}
			}

		}
	}
	return;
}

int main() {
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	int N, S, F, M;
	fscanf(input, "%d%d%d%d", &N, &S, &F, &M);

	if (!valid(output, N, M, S, F)) {
		return 0;
	}
	int** table = table_ctor(N);
	table = fill_table(input, output, table, M, N);
	if (!table) {
		return 0;
	}

	vertex* vertexes = vertex_ctor(N);
	dijkstra_path(table, vertexes, S - 1, F - 1, N);
	print_all_distance(output, vertexes, N);
	print_path(output, table, vertexes, S - 1, F - 1, N);
	vertex_and_table_dtor(vertexes, table, N);

	return 0;
}
