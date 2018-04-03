#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

typedef struct stack {
	triple val;
	struct stack* prev;
}stack;

typedef struct stackContainer {
	stack* firstInStack;
	stack* lastInStack;
}stackContainer;

typedef struct memPool{
	uint8_t *mem;
	uint32_t totalSize;
	uint32_t offset;
}memPool;

memPool memPoolConstructor(uint16_t sizeOfStruct, uint32_t lengthOfPool) {
	memPool pool;
	pool.mem = (uint8_t*)malloc(sizeOfStruct*lengthOfPool);
	pool.offset = 0;
	pool.totalSize = sizeOfStruct *lengthOfPool;
	return pool;
}

void* getMem(memPool* buf, uint32_t size) {
	if (buf == NULL || size == NULL)
		return NULL;
	uint32_t newOffset = buf->offset + size;
	if (newOffset <= buf->totalSize) {
		void* ptr = buf->mem + buf->offset;
		buf->offset = newOffset;
		return ptr;
	}
	return NULL;
}

stackContainer stackContainerHolder(triple t, stackContainer graphContainer, memPool* memPool) {
	stack* st = (stack*)getMem(memPool, sizeof(stack));
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
	stack* tmp = graphContainer.firstInStack;
	while (tmp->prev != NULL) {
		if (t.length <= tmp->prev->val.length) {
			st->prev = tmp->prev;
			tmp->prev = st;
			return graphContainer;
		}
		tmp = tmp->prev;
	}
}

int isCorrectNumVerNumEd(const unsigned int numVer, const unsigned int numEd) {
	if (numVer == 1)
		return 0;
	if (numEd == 0)
	{
		printf("no spanning tree");
		return 0;
	}
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

stack* readGraph(FILE* in, unsigned int numVer, unsigned int numEd) {
	memPool pool = memPoolConstructor(sizeof(stack), numEd);

	stackContainer graphContainer;
	graphContainer.firstInStack = NULL;
	graphContainer.lastInStack = NULL;

	unsigned int i = 0;
	while (1) {
		unsigned int start, end;
		long long length;

		fscanf(in, "%d %d %lld", &start, &end, &length);

		if (feof(in)) {
			if (i < numEd)
			{
				printf("bad number of lines");
				return NULL;
			}
			break;
		}
		if (!isCorrectVerEdLength(start, end, length, numVer)) return NULL;
		++i;
		graphContainer = stackContainerHolder(tripleConstr(start - 1, end - 1, (unsigned int)length), graphContainer, &pool);
		stack* tmp = graphContainer.firstInStack;
	}
	return graphContainer.firstInStack;

}

triple** kruskal(stack* graph, unsigned int numVer, unsigned int numEd) {
	triple** result = (triple**)malloc(sizeof(triple*)*numVer);
	for (unsigned int i = 0; i < numVer; ++i) {
		result[i] = NULL;
	}

	unsigned int* treeNum = (unsigned int*)malloc(sizeof(unsigned int)*numVer);
	for (unsigned int i = 0; i < numVer; ++i)
		treeNum[i] = i;
	int num = 0;
	while (graph != NULL) {
		if (treeNum[graph->val.start] != treeNum[graph->val.end]) {
			for (int i = 0; i < numVer; ++i) {
				if (result[i] == NULL) {
					result[i] = &(graph->val);
					break;
				}
			}
			num++;
			unsigned int oldNum = treeNum[graph->val.end], newNum = treeNum[graph->val.start];
			for (unsigned int i = 0; i < numVer; ++i) {
				if (treeNum[i] == oldNum)
					treeNum[i] = newNum;
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

void printKruskalResult(FILE* out, triple** kruskalResult, unsigned int numVer) {
	if (kruskalResult != NULL)
		for (unsigned int i = 0; i < numVer; ++i) {
			if ((kruskalResult[i]) == NULL)
				break;
			if ((*(kruskalResult[i])).start >(*(kruskalResult[i])).end)
				swap(&((*(kruskalResult[i])).start), &((*(kruskalResult[i])).end));
			fprintf(out, "%d %d\n", (*(kruskalResult[i])).start + 1, (*(kruskalResult[i])).end + 1);
		}
}

int main() {
	FILE* in = fopen("in.txt", "r");
	unsigned int numVer, numEd;

	fscanf(in, "%d %d" , &numVer, &numEd);

	if (feof(in)) {
		printf("bad number of lines");
		return 0;
	}
	if (!isCorrectNumVerNumEd(numVer, numEd)) return 0;

	stack* graph = readGraph(in, numVer, numEd);
	if (graph == NULL)
		return 0;
	fclose(in);
	triple** kruskalResult = kruskal(graph, numVer, numEd);

	FILE* out = fopen("out.txt", "w");
	printKruskalResult(out, kruskalResult, numVer);
	return 0;
}
