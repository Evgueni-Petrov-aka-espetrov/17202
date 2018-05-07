#include <stdio.h>
#include <stdlib.h>

typedef struct l_edge
{
	unsigned short v_ind1;
	unsigned short v_ind2;
	unsigned int e_len;
} l_edge;

typedef struct a_edge
{
	unsigned short v1;
	unsigned short v2;
} a_edge;

void swap(l_edge arr[], int i, int j) {
	l_edge tmp;
	tmp.e_len = arr[j].e_len;
	tmp.v_ind1 = arr[j].v_ind1;
	tmp.v_ind2 = arr[j].v_ind2;
	arr[j].e_len = arr[i].e_len;
	arr[j].v_ind1 = arr[i].v_ind1;
	arr[j].v_ind2 = arr[i].v_ind2;
	arr[i].e_len = tmp.e_len;
	arr[i].v_ind1 = tmp.v_ind1;
	arr[i].v_ind2 = tmp.v_ind2;
}

int getp(const l_edge arr[], int L, int R) {
	return arr[(L + R) / 2].e_len;
}

int partition(l_edge arr[], int L, int R) {
	int p = getp(arr, L, R);
	int i = L;
	int j = R;
	while (i <= j) {
		while (arr[i].e_len < p) {
			i++;
		}
		while (p < arr[j].e_len) {
			j--;
		}
		if (i <= j) {
			swap(arr, i, j);
			i++;
			j--;
		}
	}
	return i;
}

void qsort(l_edge arr[], int L, int R) {
	if (L < R) {
		int p = partition(arr, L, R);
		qsort(arr, L, p - 1);
		qsort(arr, p, R);
	}
}


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

int read_edges(int N, int M, l_edge** head, l_edge* LE) {
	int from, to;
	*head = NULL;
	int edge_skiped = 0;
	for (int i = 0; i < M; i++) {
		if (scanf_s("%d %d %d", &(LE[i].v_ind1), &(LE[i].v_ind2), &(LE[i].e_len)) != 3) {
			puts("bad number of lines");
			return 3;
		}
		if (LE[i].e_len <0 || LE[i].e_len > INT_MAX) {
			puts("bad length");
			return 2;
		}
		if (LE[i].v_ind1 <1 || LE[i].v_ind2 <1 || LE[i].v_ind1 >N || LE[i].v_ind2 >N) {
			puts("bad vertex");
			return 2;
		}
	}
	qsort(LE, 0, M - 1);
	return 0;
}

unsigned char get_b(const unsigned char* arr, int N, unsigned short v1, unsigned short v2) {
	if (v1>v2) {
		unsigned short k = v2;
		v2 = v1;
		v1 = k;
	}
	int idx = (v1 - 1)*N - (v1 - 1)*v1 / 2 + v2 - v1 - 1;
	return (arr[idx / 8] >> (idx % 8)) & 1;
}

void set_b(unsigned char* arr, int N, unsigned short v1, unsigned short v2) {
	if (v1>v2) {
		unsigned short k = v2;
		v2 = v1;
		v1 = k;
	}
	int idx = (v1 - 1)*N - (v1 - 1)*v1 / 2 + v2 - v1 - 1;
	arr[idx / 8] |= 1 << (idx % 8);
}

int build_mst(int N, int M, l_edge* LE, a_edge *edges) {
	if (N == 1 && M == 0)
		return 0;
	unsigned char* arr;
	arr = (unsigned char*)malloc(sizeof(unsigned char)*(N*(N - 1) / 2 + 7) / 8);
	for (int i = 0; i < (N*(N - 1) / 2 + 7) / 8; i++) {
		arr[i] = 0;
	}

	int e_count = 0;
	for (int i = 0; i < M; i++) {
		if (LE[i].v_ind1 == LE[i].v_ind2) continue;
		if (get_b(arr, N, LE[i].v_ind1, LE[i].v_ind2) == 0) {
			edges[e_count].v1 = LE[i].v_ind1;
			edges[e_count].v2 = LE[i].v_ind2;
			set_b(arr, N, LE[i].v_ind1, LE[i].v_ind2);
			for (int v = 1; v <= N; v++) {
				if (v != LE[i].v_ind1 && v != LE[i].v_ind2) {
					if (get_b(arr, N, LE[i].v_ind1, v) == 1) {
						set_b(arr, N, LE[i].v_ind2, v);
					}
					if (get_b(arr, N, LE[i].v_ind2, v) == 1) {
						set_b(arr, N, LE[i].v_ind1, v);
					}
				}
			}
			e_count++;
			if (e_count == N - 1)
				return 0;
		}
	}
	puts("no spanning tree");
	return 1;
}

int main() {
	int N, M;
	if (read_head(&N, &M) != 0)
		return 0;
	l_edge* LE;
	l_edge* head;
	LE = (l_edge*)malloc(sizeof(l_edge)*M);
	if (read_edges(N, M, &head, LE) != 0)
		return 0;
	a_edge* edges = (a_edge*)malloc(sizeof(a_edge)*(N - 1));
	if (build_mst(N, M, LE, edges) != 0)
		return 0;
	for (int i = 0; i < N - 1; i++) {
		printf("%d %d\n", edges[i].v1, edges[i].v2);
	}
	free(LE);
	free(edges);
	return 0;
}