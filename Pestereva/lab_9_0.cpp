#include <stdio.h>
#include <stdlib.h>

int read_head(int *N, int *M, int *S, int *T) {
	if (scanf_s("%d", N) < 1) {
		puts("bad number of lines");
		return 1;
	}
	if (*N < 0 || *N > 5000) {
		puts("bad number of vertices");
		return 2;
	}
	if (scanf_s("%d %d", S, T) < 2) {
		puts("bad number of lines");
		return 1;
	}
	if (*S < 1 || *S > *N || *T < 1 || *T > *N) {
		puts("bad vertex");
		return 3;
	}
	(*S)--; (*T)--;
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
	if (v1 == v2) return;
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

int search_path(int N, int M, int S, const int* aw, short* edges, unsigned int* min_w) {
	//if (N == 1 && M == 0)
	//	return 0;
	short* min_edges;
	min_edges = (short*)malloc(sizeof(short)*N);
	for (int i = 0; i < N; i++) {
		edges[i] = -1;
		min_w[i] = UINT_MAX;
	}
	min_w[S] = 0;
	min_edges[S] = S;
	edges[S] = S;
	short min = -1;
	for (int v = S; min != S; v = min) {
		min = S;
		for (int w = 0; w < N; w++) {
			if (edges[w] == -1) {
				unsigned int wt = get_w(aw, N, v, w);
				if (wt != UINT_MAX) {
					if (min_w[v] <= INT_MAX) wt += min_w[v];
					else wt = UINT_MAX - 2;
				}
				if (wt>=0 && wt != UINT_MAX && wt <= min_w[w]) {
					min_w[w] = wt > INT_MAX ? min_w[w] - 1 : wt;
					min_edges[w] = v;
				}
				if (min_w[w] < min_w[min] || min == S) {
					min = w;
				}
			}
		}
		if (min != S) {
			edges[min] = min_edges[min];
		}
	}
	free(min_edges);
	return 0;
}

int main() {
	int N, M, S, T;
	if (read_head(&N, &M, &S, &T) != 0)
		return 0;
	int* AW;
	AW = (int*)malloc(sizeof(int)*N*(N - 1) / 2);
	if (read_edges(N, M, AW) != 0)
		return 0;
	short* edges;
	edges = (short*)malloc(sizeof(short)*N);
	unsigned int* min_w;
	min_w = (unsigned int*)malloc(sizeof(unsigned int)*N);
	if (search_path(N, M, S, AW, edges, min_w) != 0)
		return 0;

	for (int i = 0; i < N; i++) {
		if (min_w[i] == UINT_MAX) {
			printf("%s ", "oo");
		}
		else if (min_w[i] > INT_MAX) {
			printf("%s ", "INT_MAX+");
		}
		else {
			printf("%d ", min_w[i]);
		}
	}
	if (min_w[T] == UINT_MAX) {
		printf("\n%s ", "no path");
	}
	else if (min_w[T] > INT_MAX && min_w[T] < UINT_MAX - 1) {
		printf("\n%s ", "overflow");
	}
	else {
		short tmp = T;
		printf("\n%d ", tmp + 1);
		while (tmp != S) {
			tmp = edges[tmp];
			printf("%d ", tmp + 1);
		}
	}
	free(min_w);
	free(AW);
	free(edges);
	return 0;
}