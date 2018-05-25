#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

enum { INF = -1 };

struct vertex {
	int min_dist;
	int used;
	int ver_min_dist;
};

vertex* vertex_ctor(int N) {
	vertex* result = (vertex*)malloc(sizeof(vertex) * N);
	for (int i = 0; i < N; ++i) {
		result[i].used = 0;
		result[i].min_dist = INF;
		result[i].ver_min_dist = -1;
	}
	return result;
}

int valid(FILE* output, int N, int M) {
	if (N == 1) { return 0; }
	if (N == 0 || M == 0) { fprintf(output, "no spanning tree"); return 0; }
	if (N < 0 || N > 5000) { fprintf(output, "bad number of vertices");  return 0; }
	if (M < 0 || M > N*(N + 1) / 2) { fprintf(output, "bad number of edges"); return 0; }
	return 1;
}

int** table_ctor(int N) {
	int** result = (int**)malloc(sizeof(int*) * N);
	for (int i = 0; i < N; ++i) {
		result[i] = (int*)malloc(sizeof(int) * N);
	}
	return result;
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

int** fill_table(FILE* input, FILE* output, int** table, int M, int N) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			table[i][j] = INF;
		}
	}
	for (int i = 0; i < M; ++i) {
		int ver1, ver2;
		long long length;
		if (fscanf(input, "%d%d", &ver1, &ver2) + fscanf(input, "%lli", &length) <3) {
			fprintf(output, "bad number of lines");
			return NULL;
		}
		if (bad_input(output, ver1, ver2, length, N)) {
			return NULL;
		}
		--ver1; --ver2;
		table[ver2][ver1] = length;
		table[ver1][ver2] = length;
	}
	return table;
}

int check_table(int** table, int N) {
	int* flags = (int*)malloc(sizeof(int) * N);
	for (int i = 0; i < N; ++i) {
		flags[i] = 0;
	}
	for (int i = 0; i < N; ++i) {
		flags[i] = 0;
	}
	for (int i = 0; i < N; ++i) {
		for (int j = i + 1; j < N; ++j) {
			if (table[i][j] != INF) {
				flags[i] = 1;
				flags[j] = 1;
			}
		}
	}
	for (int i = 0; i < N; ++i) {
		if (flags[i] == 0) {
			free(flags);
			return 0;
		}
	}
	free(flags);
	return 1;
}

void vertexes_and_table_dtor(vertex* vertexes, int** table, int N) {
	for (int i = 0; i < N; ++i) {
		free(table[i]);
	}
	free(table);
	free(vertexes);
	return;
}

void make_MST(FILE* output, int** table, int N) {
	if (!check_table(table, N)) {
		fprintf(output, "no spanning tree");
		return;
	}
	vertex* vertexes = vertex_ctor(N); 
	for (int i = 0; i < N; ++i) {
		int min_dist = -1;
		int cur_ver;
		for (int j = 0; j < N; ++j) {
			if (!vertexes[j].used && (min_dist == INF || (min_dist != INF && vertexes[j].min_dist < min_dist))) {
				cur_ver = j;
				min_dist = vertexes[j].min_dist;
			}
		}
		vertexes[cur_ver].used = 1;
		if (vertexes[cur_ver].ver_min_dist != -1) {
			fprintf(output, "%d %d\n", vertexes[cur_ver].ver_min_dist + 1, cur_ver + 1);
		}
		for (int j = 0; j < N; ++j) {
			if (!vertexes[j].used && (table[cur_ver][j] < vertexes[j].min_dist || vertexes[j].min_dist == INF) && table[cur_ver][j] != INF) {
				vertexes[j].min_dist = table[cur_ver][j];
				vertexes[j].ver_min_dist = cur_ver;
			}
		}
	}
	vertexes_and_table_dtor(vertexes, table, N);
	return;
}

int main() {
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	int N, M;
	fscanf(input, "%d%d", &N, &M);
	if (!valid(output, N, M)) {
		return 0;
	}
	int** table = table_ctor(N);
	table = fill_table(input, output, table, M, N);
	if (!table) {
		return 0;
	}
	make_MST(output, table, N);
	return 0;
}
