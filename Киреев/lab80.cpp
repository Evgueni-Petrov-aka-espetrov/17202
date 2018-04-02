#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void swap(unsigned int *a, unsigned int *b) {
	unsigned int buffer = *a;
	*a = *b;
	*b = buffer;
	return;
}

typedef struct triple {
	unsigned int start;
	unsigned int end;
	unsigned int length;
}triple;

triple tripleConstr(unsigned int start, unsigned int end, unsigned int length) {
	triple tr;
	tr.start = start;
	tr.end = end;
	tr.length = length;
	return tr;
}

typedef struct stack3 {
	triple val;
	struct stack3* prev;
}stack3;

typedef struct stack3Container {
	stack3* firstInStack;
	stack3* lastInStack;
}stack3Container;

int isCorrectNumVerNumEd(const unsigned int numVer, const unsigned int numEd) {
	if (numVer < 0 || numVer > 5000) {
		printf("bad number of vertices");
		return 0;
	}
	if (numEd < 0 || numEd >(numVer*(numVer + 1) / 2)) {
		printf("bad number of edges");
		return 0;
	}
	return 1;
}

int isCorrectVerEdLength(unsigned int start, unsigned int end, long long length, unsigned int numVer) {
	if (start < 1 || start > numVer || end < 1 || end > numVer) {
		printf("bad vertex");
		return 0;
	}
	if (length < 0 || length > INT_MAX) {
		printf("bad length");
		return 0;
	}
	return 1;
}

stack3Container stack3ContainerHolder(triple t, stack3Container graphContainer) {
	stack3* st = (stack3*)malloc(sizeof(stack3));
	st->val = t;
	st->prev = NULL;
	if (graphContainer.firstInStack == NULL) {
		st->prev = graphContainer.firstInStack;
		graphContainer.firstInStack = st;
		graphContainer.lastInStack = st;
		return graphContainer;
	}
	if (t.length >= graphContainer.lastInStack->val.length) {
		graphContainer.lastInStack->prev = st;
		graphContainer.lastInStack = st;
		return graphContainer;
	}
	if (t.length <= graphContainer.firstInStack->val.length) {
		st->prev = graphContainer.firstInStack;
		graphContainer.firstInStack = st;
		return graphContainer;
	}
	stack3* tmp = graphContainer.firstInStack;
	while (tmp->prev != NULL) {
		if (t.length <= tmp->prev->val.length) {
			st->prev = tmp->prev;
			tmp->prev = st;
			return graphContainer;
		}
		tmp = tmp->prev;
	}
}

stack3* readGraph(FILE* in, unsigned int numVer, unsigned int numEd) {
	stack3* graph = NULL;
	stack3Container graphContainer;
	graphContainer.firstInStack = graph;
	graphContainer.lastInStack = graph;
	unsigned int i = 0;
	while (true) {
		unsigned int start, end;
		long long length;

		fscanf(in, "%d %d %lld", &start, &end, &length);
		if (feof(in) && i < numEd) {
			printf("bad number of lines");
			return NULL;
		}
		if (!isCorrectVerEdLength(start, end, length, numVer)) return NULL;
		++i;
		graphContainer = stack3ContainerHolder(tripleConstr(start - 1, end - 1, (unsigned int)length), graphContainer);
		if (feof(in)) {
			if (i < numEd)
			{
				printf("bad number of lines");
				return NULL;
			}
			break;
		}
	}
	graph = graphContainer.firstInStack;
	return graph;
	
}

triple** kruskal(stack3* graph, unsigned int numVer, unsigned int numEd) {
	triple** result = (triple**)malloc(sizeof(triple*)*numVer*2);
	for (unsigned int i = 0; i < numVer * 2; ++i) {
		result[i] = NULL;
	}

	unsigned int* tree_id = (unsigned int*)malloc(sizeof(unsigned int)*numVer);
	for (unsigned int i = 0; i < numVer; ++i)
		tree_id[i] = i;
	int num = 0;
	while (graph != NULL) {
		if (tree_id[graph->val.start] != tree_id[graph->val.end]) {
			for (int i = 0; i < numVer*2; ++i) {
				if (result[i] == NULL) {
					result[i] = &(graph->val);
					break;
				}
			}
			num++;
			unsigned int old_id = tree_id[graph->val.end], new_id = tree_id[graph->val.start];
			for (unsigned int i = 0; i < numVer; ++i) {
				if (tree_id[i] == old_id)
					tree_id[i] = new_id;
			}
		}
		graph = graph->prev;
	}
	if (num < numVer - 1) {
		printf("no spanning tree");
		return NULL;
	} 
	return result;
}

int main() {
	FILE* in = fopen("in.txt", "r");
	unsigned int numVer, numEd;

	fscanf(in, "%d %d" , &numVer, &numEd);
	if (numVer == 1)
		return 0;
	if (numEd == 0)
	{
		printf("no spanning tree");
		return 0;
	}
	if (feof(in)) {
		printf("bad number of lines");
		return 0;
	}
	if (!isCorrectNumVerNumEd(numVer, numEd)) return 0;

	stack3* graph = readGraph(in, numVer, numEd);
	if (graph == NULL)
	{
		return 0;
	}
	triple** kruskalRes = kruskal(graph, numVer, numEd);
	FILE* out = fopen("out.txt", "w");
	if (kruskalRes != NULL)
	for (unsigned int i = 0; i < numVer*2; ++i) {
		if ((kruskalRes[i]) == NULL)
			break;
		if ((*(kruskalRes[i])).start > (*(kruskalRes[i])).end) swap(&((*(kruskalRes[i])).start), &((*(kruskalRes[i])).end));
		fprintf(out, "%d %d\n", (*(kruskalRes[i])).start + 1, (*(kruskalRes[i])).end + 1);
	}
	return 0;
}
