#include <stdio.h>
#include <stdlib.h>

int read_head(int *N, int *M) {
	if (scanf_s("%d", N) < 1) {
		puts("bad number of lines");
		return 1;
	}
	if (*N < 0 || *N > 5000) {
		puts("bad number of vertices");
		return 2;
	}
	if (scanf_s("%d", M) < 1) {
		puts("bad number of lines");
		return 1;
	}
	if (*M < 0 || *M > *N*(*N - 1) / 2) {
		puts("bad number of edges");
		return 3;
	}
	return 0;
}

int get_w(const int* aw, int N, short v1, short v2) {
	if (v1 > v2) {
		short k = v2;
		v2 = v1;
		v1 = k;
	}
	return aw[v1*N - v1 * (v1 + 1) / 2 + v2 - v1 - 1];
}
void set_w(int* aw, int N, short v1, short v2, int w) {
	if (v1 > v2) {
		short k = v2;
		v2 = v1;
		v1 = k;
	}
	aw[v1*N - v1*(v1+1) / 2 + v2 - v1 - 1] = w;
}

int read_edges(int N, int M, int* aw) {
	int from, to;
	int w;
	for (int i = 0; i < N*(N - 1) / 2; i++) {
		aw[i] = -1;
	}
	for (int i = 0; i < M; i++) {
		if (scanf_s("%d %d %d", &from, &to, &w) != 3) {
			puts("bad number of lines");
			return 3;
		}
		if (w < 0 || w > INT_MAX) {
			puts("bad length");
			return 2;
		}
		if (from <1 || to <1 || from >N || to >N) {
			puts("bad vertex");
			return 2;
		}
		set_w(aw, N, from-1, to-1, w);
	}
	return 0;
}

int build_mst(int N, int M, const int* aw, short* edges) {
	if (N == 1 && M == 0)
		return 0;
	short* min_edges;
	unsigned int* min_w;
	min_edges = (short*)malloc(sizeof(short)*N);
	min_w = (unsigned int*)malloc(sizeof(unsigned int)*N);
	for (int i = 0; i < N; i++) {
		edges[i] = -1;
		min_w[i] = UINT_MAX;
	}
	int e_count = 0;
	int min = -1;
	for (int v = 0; min != 0; v = min) {
		min = 0;
		for (int w = 1; w < N; w++) {
			if (edges[w] == -1) {
				int wt = get_w(aw, N, v, w);
				if (wt>=0 && wt < min_w[w]) {
					min_w[w] = wt;
					min_edges[w] = v;
				}
				if (min_w[w] < min_w[min]) {
					min = w;
				}
			}
		}
		if (min > 0) {
			edges[min] = min_edges[min];
			e_count++;
		}
	}
	free(min_edges);
	free(min_w);
	if (e_count != N - 1) {
		puts("no spanning tree");
		return 1;
	}
	return 0;
}

int main() {
	int N, M;
	if (read_head(&N, &M) != 0)
		return 0;
	int* AW;
	AW = (int*)malloc(sizeof(int)*N*(N - 1) / 2);
	if (read_edges(N, M, AW) != 0)
		return 0;
	short* edges;
	edges = (short*)malloc(sizeof(short)*N);
	if (build_mst(N, M, AW, edges) != 0)
		return 0;
	for (int i = 1; i < N; i++) {
		printf("%d %d\n", edges[i]+1, i+1);
	}
	free(AW);
	free(edges);
	return 0;
}