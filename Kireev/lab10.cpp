#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct offsetTable {
	char ch;
	unsigned int val;
};
char* readTemp(FILE* stream, int* size) {
	char* result = NULL;
	char ch = 'o';
	while (ch != '\n') {
		ch = getc(stream);
		result = (char*)realloc(result, sizeof(char) + sizeof(char)*(*size));
		++*size;
		result[*size - 1] = ch;
	}
	--*size;
	return result;
}
struct offsetTable* buildTable(const char* str, const int size) {
	struct offsetTable* table = (struct offsetTable*)malloc(sizeof(struct offsetTable)*size);
	int tableSize = 1;
	for (int i = 0; i < size; ++i) {
		table[i].ch = str[i];
		table[i].val = size - i;
	}
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (table[i].ch == table[j].ch && table[i].val > table[j].val)
			{
				table[j].val = table[i].val;
			}
		}
	}
	return table;
}
char* readStr(FILE* stream, char* str, const int size, const int num, int* chCounter) {
	int i = 0;
	if (str[0] != -1) {
		for (; i < size - num; ++i) {
			str[i] = str[i + num];
		}
	}
	for (; i < size; ++i) {
		str[i] = getc(stream);
		++*chCounter;
	}
	return str;
}
int findMatches(const struct offsetTable* tempTable, const int tempSize, const char* str, const int chCounter, FILE* outStream) {
	for (int i = tempSize - 1; i >= 0; --i) {
		if (str[i] == tempTable[i].ch) {
			fprintf(outStream, "%d ", chCounter - tempSize + 1 + i);
		}
		if (str[i] != tempTable[i].ch) {
			fprintf(outStream, "%d ", chCounter - tempSize + 1 + i);
			for (int j = tempSize - 1; j >= tempSize - 1 - i; --j) {
				if (str[tempSize - 1] == tempTable[j].ch) {
					if (tempTable[j].val - 1 == 0) return tempSize;
					return tempTable[j].val - 1;
				}
			}
			return tempSize;
		}
	}
	return tempSize;
}
void boyerMoore(FILE* stream, char* temp, const int tempSize) {
	FILE* outStream = fopen("out.txt", "w");
	struct offsetTable* tempTable = buildTable(temp, tempSize);
	char* str = (char*)malloc(sizeof(char)*tempSize);
	str[0] = -1;
	int chCounter = 0;
	str = readStr(stream, str, tempSize, tempSize, &chCounter);
	while (!feof(stream)) {
		int a = findMatches(tempTable, tempSize, str, chCounter, outStream);
		str = readStr(stream, str, tempSize, a, &chCounter);
	}
	free(str);
	free(tempTable);
	fclose(outStream);
}
void main() {
	FILE* stream = fopen("in.txt", "r");
	char* temp = NULL;
	int tempSize = 0;
	temp = readTemp(stream, &tempSize);
	boyerMoore(stream, temp, tempSize);
	free(temp);
	fclose(stream);
}
