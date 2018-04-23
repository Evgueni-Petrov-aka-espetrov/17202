#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

typedef struct myPair {
	unsigned int first;
	unsigned int second;
}myPair;

typedef struct myPairStack {
	myPair val;
	struct myPairStack* prev = NULL;
}myPairStack;

myPairStack* myPairStackConstructor(myPair val, myPairStack* prev) {
	myPairStack* p = (myPairStack*)malloc(sizeof(myPairStack));
	p->val = val;
	p->prev = prev;
	return p;
}

typedef struct list {
	unsigned int val;
	struct list* next = NULL;
}list;

list* listConstructor(int val, list* l) {
	list* p = (list*)malloc(sizeof(list));
	p->val = val;
	p->next = NULL;
	if (l == NULL) return p;
	list* tmp = l;
	while (l->next != NULL)
		l = l->next;
	l->next = p;
	return tmp;
}

myPairStack* getElement(myPairStack* st, const unsigned int it) {
	for (unsigned int i = 0; i < it; ++i) {
		if (st->prev == NULL || st == NULL)
			return NULL;
		if (st->val.second == it) return st;
		st = st->prev;
	}
	return st;
}

myPair makemyPair(const unsigned int first, const unsigned int second) {
	myPair p;
	p.first = first;
	p.second = second;
	return p;
}

void myPairStackDestructor(myPairStack* p) {
	if (p->prev != NULL)
		myPairStackDestructor(p->prev);
	free(p);
}

int isBadIn(const unsigned int v1, const unsigned int v2, const unsigned int numVer) {
	if (v1 < 1 || v1 > numVer || v2 < 1 || v2 > numVer) {
		printf("bad vertex");
		return 1;
	}
	return 0;
}

int isCorrect(const unsigned int numVer, const unsigned int start, const unsigned int second, const unsigned int numEd) {
	if (numVer < 0 || numVer > 5000) {
		printf("bad number of vertices");
		return 0;
	}
	if (numEd < 0 || numEd > (numVer*(numVer+1)/2)) {
		printf("bad number of edges");
		return 0;
	}
	if (start < 1 || start > numVer || second < 1 || second > numVer) {
		printf("bad vertex");
		return 0;
	}
	return 1;
}

int charToInt(const char c) {
	if (c == '-') return -2;
	const char digit[] =
	{ '0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
	};

	for (unsigned int j = 0; j < 10; ++j) {
		if (digit[j] == c) {
			return j;
		}
	}
	return -1;
}

int readLength(FILE* in, unsigned int* length) { // велосипед чтения длины и проверки на переполнения unsigned int
	*length = 0;
	int foundNum = 0;
	while (1) {
		char i = charToInt(fgetc(in));
		if (i == -2) {
			printf("bad length");
			return 0;
		}
		if (!foundNum && i == -1) continue;
		if (feof(in) || (foundNum && i == -1)) break;
		foundNum = 1;
		if (*length * 10 > INT_MAX - i) {
			printf("bad length");
			return 0;
		}
		*length = *length * 10 + i;

		
	}
	return 1;
}

myPairStack** readGraph(FILE* in, const unsigned int numVer, unsigned int const numEd) {
	myPairStack** graph = (myPairStack**)malloc(sizeof(myPairStack*) * numVer);
	memset(graph, NULL, sizeof(myPairStack*) * numVer);

	for (unsigned int i = 0; i < numEd; ++i) {
		unsigned int v1, v2;
		long int length;
		fscanf(in, "%d %d %ld", &v1, &v2, &length);
		if (length > INT_MAX || length < 0) {
			printf("bad length");
			return NULL;
		}
		if (isBadIn(v1, v2, numVer)) return NULL;
		graph[v1 - 1] = myPairStackConstructor(makemyPair(length, v2 - 1), graph[v1 - 1]);
		graph[v2 - 1] = myPairStackConstructor(makemyPair(length, v1 - 1), graph[v2 - 1]);
		if (feof(in) && i < numEd - 1) {
			printf("bad number of lines");
			return NULL;
		}
	}
	return graph;
}

void writeAllPaths(FILE* out, unsigned int* dist, const unsigned int numVer) {
	for (unsigned int i = 0; i < numVer; ++i) {
		if (dist[i] == UINT_MAX) {
			fprintf(out, "oo ");
			continue;
		}
		if (dist[i] > INT_MAX) {
			fprintf(out, "INT_MAX+ ");
			continue;
		}
		fprintf(out, "%d ", dist[i]);
	}
	fprintf(out, "\n");
}

void writePathToEnd(FILE* out, myPairStack** graph, unsigned int *dist, unsigned int* p, const int start, const int end) {
	if (dist[end] == UINT_MAX) {
		fprintf(out, "no path");
		return;
	}
	if (dist[end] > INT_MAX) {
		myPairStack* tmp = graph[end];
		while (tmp != NULL) {
			if ((dist[graph[end]->val.second] = dist[tmp->val.second]) && (graph[end]->val.second != tmp->val.second)) {
				fprintf(out, "overflow");
				return;
			}
			tmp = tmp->prev;
		}
	}
	for (int v = end; v != start; v = p[v]) {
		fprintf(out, "%d ", v + 1);
	}
	fprintf(out, "%d ", start + 1);
}

void DijkstraAndWriteAnswerLOL(myPairStack** graph, const unsigned int numVer, const unsigned int numEd, const unsigned int start, const unsigned int end) {
	unsigned int* dist = (unsigned int*)malloc(sizeof(unsigned int)*numVer);
	unsigned int* path = (unsigned int*)malloc(sizeof(unsigned int)*numVer);
	memset(dist, UINT_MAX, sizeof(unsigned int)*numVer);

	int* used = (int*)malloc(sizeof(int)*numVer);
	memset(used, 0, sizeof(int)*numVer);
	
	dist[start] = 0;

	for (unsigned int i = 0; i < numVer; ++i) {
		int v = -1;

		for (unsigned int j = 0; j < numVer; ++j) {
			if (!(used[j]) && ((v == -1) || dist[j] < dist[v]))
				v = j;
		}
		
		if (dist[v] == UINT_MAX)
			break;
		used[v] = 1;

		myPairStack* st = graph[v];
		while (st != NULL) {
			unsigned int to = st->val.second;
			unsigned int tmpLength = 0;
			if (dist[v] > INT_MAX - st->val.first) {
				tmpLength = UINT_MAX -1;
			}
			else {
				tmpLength = dist[v] + st->val.first;
			}
			
			if (tmpLength < dist[to]) {
				dist[to] = tmpLength;
				path[to] = v;
			}
			st = st->prev;
		}
		
	}
	FILE* out = fopen("out.txt", "w");
	writeAllPaths(out, dist, numVer);
	writePathToEnd(out, graph, dist, path, start, end);
}

int main() {
	FILE* in = fopen("in.txt", "r");
	unsigned int numVer, start, second, numEd;
	fscanf(in, "%d %d %d %d", &numVer, &start, &second, &numEd);
	if (!isCorrect(numVer, start, second, numEd)) return 0;

	myPairStack** graph = readGraph(in, numVer, numEd);
	if (graph == NULL) {
		return 0;
	}
	DijkstraAndWriteAnswerLOL(graph, numVer, numEd, start - 1, second - 1);
	return 0;
}
