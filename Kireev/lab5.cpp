#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#define MAX_NUM_OF_SYM 256

typedef struct freqPair {
	uint8_t symbol;
	uint32_t number;
}freqPair;

typedef struct codeTriple {
	uint8_t symbol;
	uint32_t code;
	uint32_t shift;
}codeTriple;

void swapPair(freqPair *a, freqPair *b) {
	freqPair buffer = *a;
	*a = *b;
	*b = buffer;
}

typedef struct binTree {
	struct binTree* left;
	struct binTree* right;
	freqPair val;
}binTree;

freqPair makeFreqPair(uint8_t symbol, uint32_t number) {
	freqPair p;
	p.number = number;
	p.symbol = symbol;
	return p;
}

codeTriple makeCodeTriple(uint8_t symbol, uint8_t code, uint8_t shift) {
	codeTriple p;
	p.code = code;
	p.symbol = symbol;
	p.shift = shift;
	return p;
}

typedef struct stack {
	stack* prev;
	codeTriple val;
}stack;

typedef struct stackContainer {
	stack* firstInStack;
	stack* lastInStack;
}stackContainer;

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

stack* stackConstructor(memPool* pool, codeTriple val, stack* prev) {
	stack* p = (stack*)getMem(pool, sizeof(stack));
	p->val = val;
	p->prev = prev;
	return p;
}

freqPair* sortMass(freqPair* mass, uint8_t len) {
	for (int i = 0; i < len; ++i) {
		if (mass[i].symbol == 0) continue;
		for (int j = 0; j < len; ++j) {
			if (mass[i].number < mass[j].number && mass[j].symbol != 0) {
				swapPair(&(mass[i]), &(mass[j]));
			}
		}
	}
	return mass;
}

freqPair* clearMassFromNulls(freqPair* mass, uint8_t len) {
	freqPair* newMass = (freqPair*)malloc(sizeof(freqPair)*len);
	int j = 0;
	for (int i = 0; i < MAX_NUM_OF_SYM; ++i) {
		if (mass[i].number == 0) continue;
		newMass[j].number = mass[i].number;
		newMass[j].symbol = mass[i].symbol;
		j++;
	}
	free(mass);
	return newMass;
}

freqPair* buildfreqTable(FILE* in, uint8_t* numOfDifferentSymbols, uint32_t* numOfSymbols) {
	freqPair* freqTable = (freqPair*)malloc(sizeof(freqPair) * MAX_NUM_OF_SYM);
	memset(freqTable, 0, sizeof(freqPair) * MAX_NUM_OF_SYM);
	(*numOfSymbols) = 0;
	while (true)
	{
		uint8_t tmp = getc(in);
		if (feof(in)) {
			if ((*numOfSymbols) != 0) {
				for (int i = 0; i < MAX_NUM_OF_SYM; ++i) {
					if (freqTable[i].number != 0)
						(*numOfDifferentSymbols)++;
				}
				return sortMass(clearMassFromNulls(freqTable, *numOfDifferentSymbols), *numOfDifferentSymbols);
			}
			else {
				return NULL;
			}
		}
		(*numOfSymbols)++;
		freqTable[tmp].symbol = tmp;
		freqTable[tmp].number++;
	}
}

void printTreeDebug(binTree* tree) {
	if (tree != NULL) {
		if (tree->left == NULL && tree->right == NULL) {
			printf("%c : %d ", tree->val.symbol, tree->val.number);
		}
		else {
			printf("left ");
			printTreeDebug(tree->left);
			printf("right ");
			printTreeDebug(tree->right);
		}
	}
}

void printTree(FILE* out, binTree* tree) {
	if (tree != NULL) {
		if (tree->left == NULL && tree->right == NULL) {
			fprintf(out, "1%c", tree->val.symbol);
		}
		else {
			fprintf(out, "0");
			printTree(out, tree->left);
			printTree(out, tree->right);
		}
	}
}

void swapBinTree(binTree* a, binTree* b) {
	binTree buffer = *a;
	*a = *b;
	*b = buffer;
}

binTree** sortForest(binTree** mass, uint8_t len) {
	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			if (mass[i]->val.number < mass[j]->val.number) {
				swapBinTree((mass[i]), (mass[j]));
			}
		}
	}
	return mass;
}

binTree* nodeInit(uint8_t symbol, uint32_t number, binTree* left, binTree* right) {
	binTree* node = (binTree*)malloc(sizeof(binTree));
	node->val.symbol = symbol;
	node->val.number = number;
	node->left = left;
	node->right = right;
	return node;
}

binTree* buildTree(freqPair* freqTable, uint8_t len){
	binTree** forest = (binTree**)malloc(sizeof(binTree*)*len);
	for (uint8_t i = 0; i < len; ++i) {
		forest[i] = nodeInit(freqTable[i].symbol, freqTable[i].number, NULL, NULL);
	}
	for (uint8_t i = 0; i + 1 < len; ++i) {
		forest[i + 1] = nodeInit(NULL, forest[i]->val.number + forest[i + 1]->val.number, forest[i], forest[i + 1]);
		sortForest(forest, len);
	}
	binTree* resultTree = forest[len - 1];
	free(forest);
	return resultTree;
}

stack* buildCodeTable(memPool* pool, binTree* tree, stack* prev, uint8_t code, uint8_t shift) {
	if (tree != NULL) {
		if (tree->left == NULL && tree->right == NULL) {
			prev = stackConstructor(pool, makeCodeTriple(tree->val.symbol, code, shift), prev);
			return prev;
		}
		else {
			shift--;
			code <<= 1;
			prev = buildCodeTable(pool, tree->left, prev, code, shift);
			code |= 1;
			prev = buildCodeTable(pool, tree->right, prev, code, shift);
		}
	}
}

void printCompressedText(FILE* in, FILE* out, stack* codeTable, uint32_t numOfSymbols, uint8_t maxShift) {
	in = fopen("in.txt", "rb");
	uint8_t buf = 0;
	uint8_t counter = 0;
	for (uint32_t i = 0; i < numOfSymbols; ++i) {
		uint8_t tmp;
		fscanf(in, "%c", &tmp);
		stack* st = codeTable;
		while (st != NULL) {
			if (st->val.symbol == tmp) {// в общем, тут исходный текст должен заколдоваться и превратиться в кодированный текст
				uint32_t code = st->val.code;
				for (int i = 0; i < maxShift - st->val.shift; ++i) {
					if (counter == 8) {
						fprintf(out, "%c", buf);
						printf(" %d ", buf);
						buf = 0;
						counter = 0;
						//continue;
					}
					/*uint8_t t = buf;
					for (int i = 0; i < 8; ++i) {
						if (t & 1 == 1) {
							printf("1");
						}
						else {
							printf("0");
						}
						t >>= 1;
					}*/
					buf = buf | ((code) & 1);
					printf("%d", code & 1);
					code >>= 1;
					buf <<= 1;
					counter += 1;
					
				}
				break;
			}
			st = st->prev;
		}
	}
}

int main() {
	FILE* in = fopen("in.txt", "rb");
	uint8_t numOfDifferentSymbols = 0;
	uint32_t numOfSymbols = 0;
	freqPair* freqTable = buildfreqTable(in, &numOfDifferentSymbols, &numOfSymbols);
	fclose(in);
	if (freqTable == NULL) return NULL;

	binTree* tree = buildTree(freqTable, numOfDifferentSymbols);
	free(freqTable);

	FILE* out = fopen("out.txt", "wb");
	printTree(out, tree);
	fprintf(out, "\n%d\n", numOfSymbols);

	memPool pool = memPoolConstructor(sizeof(stack), numOfDifferentSymbols);
	int maxShift = 32;
	stack* codeTable = buildCodeTable(&pool, tree, NULL, 0, maxShift);
	free(tree);

	//stack* tmp = codeTable;
	//while (tmp != NULL) {
	//	printf("%c : %d << %d\n", tmp->val.symbol, tmp->val.code, tmp->val.shift);
	//	tmp = tmp->prev;
	//}
	printCompressedText(in, out, codeTable, numOfSymbols, maxShift);

	fclose(out);
	system("pause");
	return 0;
}
