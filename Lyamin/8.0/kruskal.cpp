#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct edge {
	int ver1, ver2;
	int length;
};

typedef struct edge edge;

edge* edge_ctor(int M) {
	edge* result = (edge*)malloc(sizeof(edge)*M);
	return result;
}

int valid(FILE* output, int N, int M) {
	if (N == 1) { return 0; }
	if (N == 0 || M == 0) { fprintf(output, "no spanning tree"); return 0; }
	if (N < 0 || N > 5000) { fprintf(output, "bad number of vertices");  return 0; }
	if (M < 0 || M > N * (N + 1) / 2) { fprintf(output, "bad number of edges"); return 0; }
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

edge* fill_table(FILE* input, FILE* output, edge* table, int M, int N) {
	for (int i = 0; i < M; ++i) {
		int ver1, ver2;
		long long length;
		if (fscanf(input, "%d%d", &ver1, &ver2) + fscanf(input, "%lli", &length) < 3) {
			fprintf(output, "bad number of lines");
			free(table);
			return NULL;
		}

		if (bad_input(output, ver1, ver2, length, N)) {
			free(table);
			return NULL;
		}

		--ver1; --ver2;
		
		table[i].ver1 = ver1;
		table[i].ver2 = ver2;
		table[i].length = length;

	}
	return table;
}

void edge_swap(edge* table, int i, int j) {
	edge buff = table[i];
	table[i] = table[j];
	table[j] = buff;
	return;
}

int partition(edge* a, int low, int high) {
	int i = low;
	int j = high;
	while (i <= j) {
		while (a[i].length < a[low].length) {
			i += 1;
		}

		while (a[j].length > a[low].length) {
			j -= 1;
		}

		if (i <= j) {
			edge_swap(a, i, j);
			i += 1;
			j -= 1;
		}
	}
	return i;
}

int compare_func(const void* first, const void* second) {
	return (*(edge*)first).length - (*(edge*)second).length;
}

void quick_sort(edge* a, int low, int high) {
	if (low < high) {
		int p = partition(a, low, high);
		quick_sort(a, low, p - 1);
		quick_sort(a, p, high);
	}
	return;
}

int check_table(edge* table, int N, int M) {
	char* flags = (char*)malloc(sizeof(char)* M);
	for (int i = 0; i < N; ++i) {
		flags[i] = 0;
	}

	for (int i = 0; i < M; ++i) {
		flags[table[i].ver1] = 1;
		flags[table[i].ver2] = 1;
	}

	for (int i = 0; i < N; ++i) {
		if (flags[i] == 0) {
			return 0;
		}
	}
	return 1;
}

void kruskal(FILE* output, edge* table, int N, int M) {
	if (!check_table(table, N, M)) {
		fprintf(output, "no spanning tree");
		free(table);
		return;
	}
	int* flags = (int*)malloc(sizeof(int) * N);
	for (int i = 0; i < N; ++i) {
		flags[i] = -1;
	}

	//edge_sort(table, M);
	//quick_sort(table, 0, M - 1);
	qsort(table, M, sizeof(edge), compare_func);

	for (int i = 0; i < M; ++i) {
		int ver1 = table[i].ver1;
		int ver2 = table[i].ver2;

		if ((flags[ver1] == flags[ver2]) && (flags[ver1] != -1)) {
			continue;
			}

		if ((flags[ver1] != -1) && (flags[ver2] != -1)) {
			int buffer = flags[ver2];
			for (int j = 0; j < N; ++j) {
				if (flags[j] == buffer) {
					flags[j] = flags[ver1];
				}
			}
			fprintf(output, "%d %d\n", ver1 + 1, ver2 + 1);
			continue;
		}

		if (flags[ver1] == -1 && flags[ver2] != -1) {
			flags[ver1] = flags[ver2];
			fprintf(output, "%d %d\n", ver1 + 1, ver2 + 1);
			continue;
		}

		if (flags[ver1] != -1 && flags[ver2] == -1) {
			flags[ver2] = flags[ver1];
			fprintf(output, "%d %d\n", ver1 + 1, ver2 + 1);
			continue;
		}

		if (flags[ver1] == -1 && flags[ver2] == -1) {	
			fprintf(output, "%d %d\n", ver1 + 1, ver2 + 1);
			flags[ver1] = ver1;
			flags[ver2] = ver1;
			continue;
		}
	}

	free(flags);
	free(table);
	return;
}

int main() {
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	int N, M;
	fscanf(input, "%d%d", &N, &M);
	if (!valid(output, N, M)) {	return 0; }
	edge* table = edge_ctor(M);
	table = fill_table(input, output, table, M, N);
	if (!table) {	return 0; }
	kruskal(output, table, N, M);
	return 0;
}
