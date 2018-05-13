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

codeTriple makeCodeTriple(uint8_t symbol, uint32_t code, uint8_t shift) {
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

typedef struct memPool {
	uint8_t *mem;
	uint32_t totalSize;
	uint32_t offset;
}memPool;

memPool memPoolConstructor(uint16_t sizeOfStruct, uint32_t lengthOfPool) {
	memPool codeTablePool;
	codeTablePool.mem = (uint8_t*)malloc(sizeOfStruct*lengthOfPool);
	codeTablePool.offset = 0;
	codeTablePool.totalSize = sizeOfStruct * lengthOfPool;
	return codeTablePool;
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

stack* stackConstructor(memPool* codeTablePool, codeTriple val, stack* prev) {
	stack* p = (stack*)getMem(codeTablePool, sizeof(stack));
	p->val = val;
	p->prev = prev;
	return p;
}

freqPair* sortMass(freqPair* mass, uint32_t len) {
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

freqPair* clearMassFromNulls(freqPair* mass, uint32_t len) {
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

freqPair* buildfreqTable(FILE* in, uint32_t* numOfDifferentSymbols, uint32_t* numOfSymbols) {
	freqPair* freqTable = (freqPair*)malloc(sizeof(freqPair) * MAX_NUM_OF_SYM);
	memset(freqTable, 0, sizeof(freqPair) * MAX_NUM_OF_SYM);
	(*numOfSymbols) = 0;
	while (1)
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

void printTree(FILE* out, binTree* tree, uint8_t* buf, uint8_t* counter) {
	if (tree != NULL) {
		if ((*counter) == 8) {
			fprintf(out, "%c", *buf);
			(*buf) = 0;
			(*counter) = 0;
		}
		if (tree->left == NULL && tree->right == NULL) {
			(*buf) <<= 1;
			*buf = (*buf) | 1;
			(*counter)++;
			if (*counter == 8) {
				fprintf(out, "%c%c", *buf, tree->val.symbol);
				*buf = 0;
				*counter = 0;
			}
			else {
				*buf = (*buf << (8 - (*counter))) | (tree->val.symbol >> (*counter));
				fprintf(out, "%c", *buf);
				*buf = (tree->val.symbol & (255 >> (8 - (*counter))));
			}
		}
		else {
			(*counter)++;
			(*buf) = (*buf) << 1;
			printTree(out, tree->left, buf, counter);
			printTree(out, tree->right, buf, counter);
		}
	}
}

void swapBinTree(binTree* a, binTree* b) {
	binTree buffer = *a;
	*a = *b;
	*b = buffer;
}

binTree** sortForest(binTree** mass, uint32_t len) {
	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			if (mass[i]->val.number < mass[j]->val.number) {
				swapBinTree((mass[i]), (mass[j]));
			}
		}
	}
	return mass;
}

binTree* nodeInit(memPool* binTreePool, uint8_t symbol, uint32_t number, binTree* left, binTree* right) {
	//binTree* node = (binTree*)malloc(sizeof(binTree));
	binTree* node = (binTree*)getMem(binTreePool, sizeof(binTree));
	node->val.symbol = symbol;
	node->val.number = number;
	node->left = left;
	node->right = right;
	return node;
}

binTree* buildTree(memPool* binTreePool, freqPair* freqTable, uint32_t len) {
	binTree** forest = (binTree**)malloc(sizeof(binTree*)*len);

	for (uint32_t i = 0; i < len; ++i) {
		forest[i] = nodeInit(binTreePool, freqTable[i].symbol, freqTable[i].number, NULL, NULL);
	}
	for (uint32_t i = 0; i + 1 < len; ++i) {
		forest[i + 1] = nodeInit(binTreePool, NULL, forest[i]->val.number + forest[i + 1]->val.number, forest[i], forest[i + 1]);
		sortForest(forest, len);
	}
	binTree* resultTree = forest[len - 1];
	free(forest);
	return resultTree;
}

stack* buildCodeTable(memPool* codeTablePool, binTree* tree, stack* prev, uint32_t code, uint8_t shift) {
	if (tree->left == NULL && tree->right == NULL) {
		prev = stackConstructor(codeTablePool, makeCodeTriple(tree->val.symbol, code, shift), prev);
		return prev;
	}
	else {
		shift--;
		code <<= 1;
		prev = buildCodeTable(codeTablePool, tree->left, prev, code, shift);
		code |= 1;
		prev = buildCodeTable(codeTablePool, tree->right, prev, code, shift);
	}
	return prev;
}

void printBytesUsingBuffer(FILE* out, uint8_t* buf, uint8_t* counter, uint8_t numOfBit, uint32_t code) {
	if (numOfBit == 255) {
		fprintf(out, "%c", 0);
		return;
	}
	for (uint8_t i = 0; i <= numOfBit; ++i) {
		*buf <<= 1;
		uint32_t pos = (1 << (numOfBit));
		*buf = *buf | (((code)& pos) >> (numOfBit));
		code <<= 1;
		*counter += 1;
		if (*counter == 8) {
			fprintf(out, "%c", *buf);
			*buf = 0;
			*counter = 0;
		}
	}
}

void printCompressedText(FILE* in, FILE* out, stack* codeTable, uint32_t numOfSymbols, uint8_t maxShift) {
	fseek(in, 3, SEEK_SET);
	uint8_t buf = 0;
	uint8_t counter = 0;

	for (uint32_t i = 0; i < numOfSymbols; ++i) {
		uint8_t tmp;
		fscanf(in, "%c", &tmp);
		stack* st = codeTable;
		while (st != NULL) {
			if (st->val.symbol == tmp) {
				printBytesUsingBuffer(out, &buf, &counter, maxShift - st->val.shift - 1, st->val.code);
				break;
			}
			st = st->prev;
		}
	}
	fprintf(out, "%c", buf << (8 - counter));
}

void fastPrintCompressedText(FILE* out, stack* codeTable, uint32_t numOfSymbols, int maxShift) {
	for (int i = 0; i < numOfSymbols / 32; ++i) {
		fprintf(out, "%c%c%c%c", 0, 0, 0, 0);
	}
	for (int i = 0; i < numOfSymbols % 32; ++i) {
		fprintf(out, "%c", 0);
	}
}

int checkСompressibility(uint32_t numOfDifferentSymbols, freqPair* freqTable) {
	if (numOfDifferentSymbols == 256) {
		uint32_t counter = 1;
		for (uint32_t i = 1; i < numOfDifferentSymbols; ++i) {
			if (freqTable[i].number == freqTable[0].number) counter++;
		}
		if (counter == numOfDifferentSymbols)
			return 0;
	}
	return 1;
}

void compressText(FILE* in, FILE* out) {
	uint32_t numOfDifferentSymbols = 0,
		numOfSymbols = 0;
	freqPair* freqTable =
		buildfreqTable(in,
			&numOfDifferentSymbols,
			&numOfSymbols);

	if (freqTable == NULL) {
		fprintf(out, "p%c%c\n0\n", 208, 0);
		return;
	}
	if (!checkСompressibility(numOfDifferentSymbols, freqTable)) {
		fprintf(out, "i");
		fseek(in, 3, SEEK_SET);
		while (1) {
			uint8_t tmp = getc(in);
			if (feof(in))
				return;
			fprintf(out, "%c", tmp);
		}
	}
	else
		fprintf(out, "p");

	memPool binTreePool =
		memPoolConstructor(sizeof(binTree),
			numOfDifferentSymbols * 2);

	binTree* tree =
		buildTree(&binTreePool,
			freqTable,
			numOfDifferentSymbols);
	free(freqTable);

	uint8_t buf = 0, counter = 0;
	printTree(out, tree, &buf, &counter);
	buf <<= 8 - counter;
	fprintf(out, "%c", buf);
	fprintf(out, "\n%d\n", numOfSymbols);

	memPool codeTablePool =
		memPoolConstructor(sizeof(stack),
			numOfDifferentSymbols);
	int maxShift = 32;
	stack* codeTable = buildCodeTable(&codeTablePool,
		tree,
		NULL,
		0,
		maxShift);

	free(binTreePool.mem);

	if (numOfDifferentSymbols > 1)
		printCompressedText(in,
			out,
			codeTable,
			numOfSymbols,
			maxShift);
	else
		fastPrintCompressedText(out,
			codeTable,
			numOfSymbols,
			maxShift);

	free(codeTablePool.mem);
}

void readNode(FILE* in, binTree* prev, uint8_t* buf, uint8_t* counter) {
	if ((*counter) == 8) {
		(*buf) = getc(in);
		(*counter) = 0;
	}
	if (((*buf) & 128) == 0) {
		prev->left = (binTree*)malloc(sizeof(binTree));
		(*buf) <<= 1;
		(*counter)++;
		readNode(in, prev->left, buf, counter);
	}
	else {
		(*buf) <<= 1;
		(*counter)++;
		if ((*counter) == 8) {
			(*buf) = getc(in);
			prev->val.symbol = (*buf);
			prev->left = NULL;
			prev->right = NULL;
			(*buf) = getc(in);
			(*counter) = 0;
			return;
		}
		uint8_t tmp = getc(in);
		(*buf) = (*buf) | (tmp >> (8 - (*counter)));
		prev->val.symbol = (*buf);
		prev->left = NULL;
		prev->right = NULL;
		(*buf) = tmp << (*counter);
		return;
	}
	prev->right = (binTree*)malloc(sizeof(binTree));
	readNode(in, prev->right, buf, counter);
}

binTree* readTree(FILE* in) {
	binTree* tree = (binTree*)malloc(sizeof(binTree));
	uint8_t buf = getc(in), counter = 0;
	readNode(in, tree, &buf, &counter);
	return tree;
}

void printDepressedText(FILE* in, FILE* out, binTree* tree, int32_t numOfSymbols) {
	uint8_t counter = 0;
	uint8_t buf = getc(in);
	for (int i = 0; i < numOfSymbols; ++i) {
		binTree* tmp = tree;
		while (tmp->left != NULL && tmp->right != NULL) {
			if (counter == 8) {
				buf = getc(in);
				counter = 0;
			}
			if ((buf & 128) == 0) {
				tmp = tmp->left;
			}
			else {
				tmp = tmp->right;
			}
			buf <<= 1;
			counter++;
		}
		fprintf(out, "%c", tmp->val.symbol);
	}
}

void incompressibleTextOut(FILE* in, FILE* out) {
	while (1) {
		uint8_t tmp = getc(in);
		if (feof(in))
			return;
		fprintf(out, "%c", tmp);
	}
}

int isOneNodeTree(binTree* tree) {
	if (tree->left == NULL && tree->right == NULL) return 1;
	return 0;
}

void oneNodeTreeOut(FILE* out, uint32_t numOfSymbols, uint8_t symbol) {
	for (uint32_t i = 0; i < numOfSymbols / 4; ++i)
		fprintf(out, "%c%c%c%c", symbol, symbol, symbol, symbol);
	for (uint32_t i = 0; i < numOfSymbols % 4; ++i)
		fprintf(out, "%c", symbol);
}

void depressText(FILE* in, FILE* out) {
	uint8_t compPoss = getc(in);
	if (compPoss == 'i') {
		incompressibleTextOut(in, out);
		return;
	}
	binTree* tree = readTree(in);

	int32_t numOfSymbols;
	fscanf(in, "%d", &numOfSymbols);

	if (isOneNodeTree(tree)) {
		oneNodeTreeOut(out, numOfSymbols, tree->val.symbol);
		return;
	}

	getc(in);
	printDepressedText(in, out, tree, numOfSymbols);
}

int main() {
	FILE* in = fopen("in.txt", "rb");
	FILE* out = fopen("out.txt", "wb");
	uint8_t whatToDo;
	fscanf(in, "%c", &whatToDo);
	getc(in), getc(in);

	if (whatToDo == 'c')
		compressText(in, out);
	else
		depressText(in, out);

	fclose(in), fclose(out);

	return 0;
}
