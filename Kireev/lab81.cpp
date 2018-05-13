#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <memory.h>

void swap(unsigned int *a, unsigned int *b) {
	unsigned int buffer = *a;
	*a = *b;
	*b = buffer;
	return;
}

typedef struct pair {
	unsigned int start;
	unsigned int end;
}pair;

pair pairConstructor(unsigned int start, unsigned int end) {
	pair p;
	p.start = start;
	p.end = end;
	return p;
}

typedef struct stack {
	stack* prev;
	pair val;
}stack;

stack* stackConstructor(pair val, stack* prev) {
	stack* st = (stack*)malloc(sizeof(stack));
	st->prev = prev;
	st->val = val;
	return st;
}

typedef struct memPool {
	uint8_t *mem;
	uint32_t totalSize;
	uint32_t offset;
}memPool;

memPool memPoolConstructor(uint16_t sizeOfStruct, uint32_t lengthOfPool) {
	memPool pool;
	pool.mem = (uint8_t*)malloc(sizeOfStruct*lengthOfPool);
	pool.offset = 0;
	pool.totalSize = sizeOfStruct * lengthOfPool;
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

unsigned int** readGraph(FILE* in, unsigned int numVer, unsigned int numEd) {
	memPool pool = memPoolConstructor(sizeof(unsigned int), numVer*numVer);
	unsigned int** graph = (unsigned int**)malloc(sizeof(unsigned int*)*numVer);
	for (int i = 0; i < numVer; ++i) {
		graph[i] = (unsigned int*)getMem(&pool, sizeof(unsigned int)*numVer);
		memset(graph[i], -1, sizeof(unsigned int)*numVer);
	}

	for (unsigned int i = 0; i < numEd; ++i) {
		unsigned int start, end;
		long long length;

		fscanf(in, "%d %d %lld", &start, &end, &length);
		if (!isCorrectVerEdLength(start, end, length, numVer)) return NULL;

		graph[start - 1][end - 1] = (unsigned int)length;
		graph[end - 1][start - 1] = (unsigned int)length;

		if (feof(in)) {
			if (i < numEd-1)
			{
				printf("bad number of lines");
				return NULL;
			}
			break;
		}
	}
	return graph;
}

int isCorrectNumVerNumEd(const unsigned int numVer, const unsigned int numEd) {
	if (numVer == 1)
		return 0;
	if (numEd == 0 || numVer == 0)
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

void* initMass(uint16_t sizeOfElement, uint32_t lengthOfMass, uint32_t value) {
	void* mass = malloc(sizeOfElement*lengthOfMass);
	memset(mass, value, sizeOfElement*lengthOfMass);
	return mass;
}

stack* algoPrim(unsigned int** graph, const unsigned int numVer, const unsigned int numEd) {
	unsigned int* used = (unsigned int*)initMass(sizeof(unsigned int), numVer, 0);
	unsigned int* minEd = (unsigned int*)initMass(sizeof(unsigned int), numVer, UINT_MAX);
	unsigned int* endOfMinEd = (unsigned int*)initMass(sizeof(unsigned int), numVer, numVer+1);
	stack* result = NULL;
	minEd[0] = 0;
	for (unsigned int i = 0; i < numVer; ++i) {
		int v = -1;
		for (unsigned j = 0; j < numVer; ++j) {
			if ((!used[j]) && (v == -1 || minEd[j] < minEd[i])) {
				v = j;
			}
		}
		if (minEd[v] == UINT_MAX){
			printf("no spanning tree");
			return NULL;
		}

		if (endOfMinEd[v] != numVer + 1 && minEd[v] <= INT_MAX) {
			result = stackConstructor(pairConstructor(v, endOfMinEd[v]), result);
		}

		used[v] = 1;
		for (unsigned int to = 0; to < numVer; ++to) {
			if (graph[v][to] < minEd[to] && graph[v][to] <= INT_MAX) {
				minEd[to] = graph[v][to];
				endOfMinEd[to] = v;
			}
		}
	}

	/*printf("used\n");
	for (unsigned int i = 0; i < numVer; ++i) {
		printf("%d: %d\n", i + 1, used[i]);
	}
	printf("minEd\n");
	for (unsigned int i = 0; i < numVer; ++i) {
		printf("%d: %u\n", i + 1, minEd[i]);
	}
	printf("endOfMinEd\n");
	for (unsigned int i = 0; i < numVer; ++i) {
		printf("%d: %d\n", i + 1, endOfMinEd[i] + 1);
	}*/
	//system("pause");
	/*for (unsigned int i = 0; i < numVer; ++i) {
		if (minEd[i] > INT_MAX) {
			printf("no spanning tree");
			return NULL;
		}	
	}*/
	//printf("result\n");
	stack* tmp = result;
	int it = 0;
	while (tmp!=NULL) {
		it++;
		//printf("%u : %u\n", tmp->val.start + 1, tmp->val.end + 1);
		tmp = tmp->prev;
	}
	//system("pause");
	if (it < numVer - 1) {
		printf("no spanning tree");
		return NULL;
	}
	free(used);
	free(minEd);
	return result;
}

void printResult(FILE* out, stack* result, unsigned int numVer) {
	if (result == NULL) return;
	/*for (unsigned int i = 0; i < numVer; i++)
	{
		if (result[i] <= numVer)
		{
			if(i+1 < result[i]+1)
				fprintf(out, "%d %d\n", i + 1, result[i] + 1);
			else {
				if(result[result[i]] != i)
					fprintf(out, "%d %d\n", result[i] + 1, i + 1);
			}
		}
	}*/
	while (result != NULL) {
		if (result->val.end <= numVer) {
			if (result->val.start < result->val.end)
				fprintf(out, "%d %d\n", result->val.start + 1, result->val.end + 1);
			else {
				fprintf(out, "%d %d\n", result->val.end + 1, result->val.start + 1);
			}
		}
		result = result->prev;
	}
}

int main() {
	FILE* in = fopen("in.txt", "r");
	unsigned int numVer, numEd;

	fscanf(in, "%d %d\n", &numVer, &numEd);
	
	if (!isCorrectNumVerNumEd(numVer, numEd)) return 0;
	if (feof(in)) {
		printf("bad number of lines");
		return 0;
	}

	unsigned int** graph = readGraph(in, numVer, numEd);
	if (graph == NULL) return 0;
	
	fclose(in);

	stack* result = algoPrim(graph, numVer, numEd);
	FILE* out = fopen("out.txt", "w");
	printResult(out, result, numVer);

	fclose(out);
	return 0;
}
