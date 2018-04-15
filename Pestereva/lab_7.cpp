#include <stdio.h>
#include <stdlib.h>


typedef struct l_num
{
	unsigned short v_ind;
	l_num* next;
} l_num;

typedef struct vertex
{
	unsigned char state;
	l_num* out;
} vertex;

l_num* insert(l_num* head, unsigned short v_ind) {
	l_num* N = (l_num*)malloc(sizeof(l_num));
	N->next = head;
	N->v_ind = v_ind;
	return N;
}

l_num* insert(l_num* head, unsigned short v_ind, l_num* N) {
	N->next = head;
	N->v_ind = v_ind;
	return N;
}

int read(vertex** V, int* len, l_num* E) {
	scanf_s("%d", len);
	if (*len < 0 || *len > 1000) {
		puts("bad number of vertices");
		return 1;
	}
	int e_num;
	if (scanf_s("%d", &e_num) < 1) {
		puts("bad number of lines");
		return 3;
	}
	if (e_num < 0 || e_num > *len*(*len - 1) / 2) {
		puts("bad number of edges");
		return 2;
	}
	*V = (vertex*)malloc(sizeof(vertex)**len);
	for (int i = 0; i < *len; i++) {
		(*V)[i].out = NULL;
		(*V)[i].state = 0;
	}
	E = (l_num *)malloc(sizeof(l_num)*e_num);
	for (int i = 0; i < e_num; i++) {
		int from, to;
		if (scanf_s("%d %d", &from, &to) != 2) {
			puts("bad number of lines");
			return 3;
		}
		if (from < 1 || to < 1 || from > *len || to> *len) {
			puts("bad vertex");
			return 4;
		}
		(*V)[from - 1].out = insert((*V)[from - 1].out, to - 1, &E[i]);
	}
	return 0;
}

int v_visit(vertex* V, int i, l_num** l_sorted) {
	if (V[i].state == 1)
		return 1;
	if (V[i].state == 2)
		return 0;
	V[i].state = 1;
	l_num* out = V[i].out;
	while (out != NULL) {
		if (v_visit(V, out->v_ind, l_sorted)!=0) {
			return 1;
		}
		out = out->next;
	}
	V[i].state = 2;
	*l_sorted = insert(*l_sorted, i);
	return 0;
}

int top_sort(vertex* V, int len, l_num** l_sorted) {
	for (int i = 0; i < len; i++) {
		if (v_visit(V, i, l_sorted) != 0) {
			return 1;
		}
	}
	return 0;

}

void clear(l_num* l_num) {
	if (l_num != NULL) {
		clear(l_num->next);
		free(l_num);
	}
}

int main() {
	vertex* V=NULL;
	int len;
	l_num* l_sorted = NULL;
	l_num* E = NULL;
	if (read(&V, &len, E) == 0) {
		if (top_sort(V, len, &l_sorted) != 0) {
			puts("impossible to sort");
		}
		else {
			l_num* n = l_sorted;
			while (n != NULL) {
				printf("%d ", n->v_ind+1);
				n = n->next;
			}
		}
	}
	clear(l_sorted);
	free(E);
	free(V);
	return 0;
}