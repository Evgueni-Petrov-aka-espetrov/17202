#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
	int frequency;
	int cLength;
	int code;
	struct tree* left;
	struct tree* right;
	unsigned char c;
} tree;

typedef struct symbol {
	int frequency;
	unsigned char c;
} symbol;

typedef struct stack {
	int cLength;
	int code;
	stack* next;
	unsigned char c;
} stack;

typedef struct bitStream {
	FILE* file;
	int counter;
	unsigned char data;
} bitStream;



tree* CreateTree(unsigned char c) {
	tree* res = (tree*)malloc(sizeof(tree));
	res->frequency = 0;
	res->cLength = 0;
	res->code = 0;
	res->left = NULL;
	res->right = NULL;
	res->c = c;
	return res;
}

stack* CreateTable(int code, int codeLength, unsigned char c) {
	stack* res = (stack*)malloc(sizeof(stack));
	res->c = c;
	res->cLength = codeLength;
	res->code = code;
	res->next = NULL;
	return res;
}

bitStream* CreateBitStream(FILE* oFile) {
	bitStream* res = (bitStream*)malloc(sizeof(bitStream));
	res->file = oFile;
	res->counter = 0;
	res->data = 0;
	return res;
}

tree** CreateForest(symbol* data, int NumOfDiffSymb) {
	tree** res = (tree**)malloc(sizeof(tree*) * NumOfDiffSymb);
	int counter = 0;
	for (int i = 0; i < 256; i++) {
		if (data[i].frequency != 0) {
			res[counter] = CreateTree(data[i].c);
			res[counter]->frequency = data[i].frequency;
			counter++;
		}
	}
	return res;
}

symbol* CreateSymbols() {
	symbol* res = (symbol*)malloc(sizeof(symbol)*256);
	for (int i = 0; i < 256; i++) {
		res[i].frequency = 0;
		res[i].c = (unsigned char)i;
	}
	return res;
}



void SwapTrees(tree** first, tree** second) {
	tree* tmp = *first;
	*first = *second;
	*second = tmp;
}

void ForestSort(tree** forest, int begin, int end) {
	for (int i = begin; i < end; i++)
		for (int j = i + 1; j < end; j++)
			if (forest[i]->frequency > forest[j]->frequency)
				SwapTrees(&forest[i], &forest[j]);
}

tree* MatchTrees(tree* first, tree* second) {
	tree* res = CreateTree(' ');
	res->frequency = first->frequency + second->frequency;
	res->left = first;
	res->right = second;
	return res;
}

tree* CreateHashTree(symbol* data, int NumOfDiffSymb) {
	tree** forest = CreateForest(data, NumOfDiffSymb);
	for (int i = 0; i < NumOfDiffSymb - 1; i++) {
		ForestSort(forest, i, NumOfDiffSymb);
		forest[i + 1] = MatchTrees(forest[i], forest[i + 1]);
	}
	return forest[NumOfDiffSymb - 1];
}



void PushTable(stack** table, unsigned char c, int cLength, int code) {
	if (!(*table))
		CreateTable(code, cLength, c);

	stack* res = CreateTable(code, cLength, c);
	res->next = *table;
	*table = res;
}

void CreateHashTable(tree* HashTree, stack** table) {
	if (!HashTree)
		return;

	if (HashTree->left)
		CreateHashTable(HashTree->left, table);

	if (HashTree->right)
		CreateHashTable(HashTree->right, table);

	if (!HashTree->right && !HashTree->left)
		PushTable(table, HashTree->c, HashTree->cLength, HashTree->code);

}



void PrintNumberOfSymb(FILE* oFile, int in_size) {
	unsigned char res[4];
	for (int i = 0; i < 4; i++) {
		res[i] = in_size & 0xff;
		in_size >>= 8;
	}
	fprintf(oFile, "%c%c%c%c", res[3], res[2], res[1], res[0]);
}

void PrintCode(bitStream* out, int code, int codeLength) {
	for (int i = 0; i < codeLength; i++) {
		if (out->counter == 8) {
			fprintf(out->file, "%c", out->data);
			out->counter = 0;
			out->data = 0;
		}
		int tmp = code >> (codeLength - 1 - i);
		code = code - (tmp << (codeLength - 1 - i));
		out->data <<= 1;
		out->data += (unsigned char)tmp;
		out->counter += 1;
	}
}

void EncodeEndPrintHashTree(bitStream* out, tree* HashTree) {
	if (!HashTree)
		return;

	if (!HashTree->left && !HashTree->right) {
		PrintCode(out, 1, 1);
		PrintCode(out, HashTree->c, 8);
		return;
	}

	if (HashTree->left) {
		PrintCode(out, 0, 1);
		EncodeEndPrintHashTree(out, HashTree->left);
	}
	if (HashTree->right) 
		EncodeEndPrintHashTree(out, HashTree->right);
	
	return;
}

stack* FindCurrentTable(stack* HashTable, unsigned char currSymb) {
	stack* res = HashTable;
	while (res) {
		if (res->c == currSymb)
			return res;
		res = res->next;
	}
	return NULL;
}

void EncodeAndPrintText(FILE* iFile, bitStream* out, stack* HashTable, int in_size) {
	for (int i = 0; i < in_size; i++) {
		unsigned char currSymb = (unsigned char)fgetc(iFile);
		stack* currTable = FindCurrentTable(HashTable, currSymb);
		PrintCode(out, currTable->code, currTable->cLength);
	}
}



int NumberOfDiffSymbols(symbol* data) {
	int res = 0;
	for (int i = 0; i < 256; i++)
		if (data[i].frequency != 0)
			res++;
	return res;
}

void SetCodesEndCLengthInTree(tree* tree) {
	if (!tree)
		return;

	if (tree->left) {
		tree->left->cLength = tree->cLength + 1;
		tree->left->code = (tree->code << 1);
		SetCodesEndCLengthInTree(tree->left);
	}
	if (tree->right) {
		tree->right->cLength = tree->cLength + 1;
		tree->right->code = (tree->code << 1) + 1;
		SetCodesEndCLengthInTree(tree->right);
	}
}

symbol* ReadAndCountSymbols(FILE* iFile, int* in_size) {
	symbol* res = CreateSymbols();
	while (1) {
		unsigned char c = (unsigned char)fgetc(iFile);
		if (feof(iFile))
			break;
		res[(int)c].frequency++;
		(*in_size)++;
	}
		
	fseek(iFile, 3, SEEK_SET);
	return res;
}

void FreeTree(tree** tree) {
	if (!(*tree))
		return;
	if ((*tree)->left)
		FreeTree(&(*tree)->left);

	if ((*tree)->right)
		FreeTree(&(*tree)->right);

	free(*tree);
}

void FreeStack(stack** table) {
	while (*table) {
		stack* tmp = (*table)->next;
		free(tmp);
		*table = tmp;
	}
	table = NULL;
}

void FreeAll(stack* table, tree* tree, bitStream* stream, symbol* data) {
	free(stream);
	free(data);
	FreeTree(&tree);
	FreeStack(&table);
}

int CanCompress(symbol* data, int NumOfDiffSymbols) {
	if (NumOfDiffSymbols == 256) {
		for (int i = 0; i < NumOfDiffSymbols; i++)
			if (data[0].frequency != data[i].frequency)
				return 1;
		return 0;
	}
	return 1;
}

void encode(FILE* iFile, FILE* oFile) {
	int in_size = 0;
	symbol* data = ReadAndCountSymbols(iFile, &in_size);
	
	int NumOfDiffSymb = NumberOfDiffSymbols(data);
	fprintf(oFile, "%c", CanCompress(data, NumOfDiffSymb));
	if (!CanCompress(data, NumOfDiffSymb))
		while (1) {
			int symb = fgetc(iFile);
			if (feof(iFile))
				return;
				
			fprintf(oFile, "%c", symb);
		}
	else {
		PrintNumberOfSymb(oFile, in_size);
		
		if (NumOfDiffSymb == 0)
			return;

		bitStream* out = CreateBitStream(oFile);
		tree* HashTree = CreateHashTree(data, NumOfDiffSymb);		
		
		EncodeEndPrintHashTree(out, HashTree);
		
		if (!HashTree->left && !HashTree->right) {
		PrintCode(out, 0, 8);
			return;
		}

		SetCodesEndCLengthInTree(HashTree);
		stack* HashTable = NULL;
		CreateHashTable(HashTree, &HashTable);
	
		EncodeAndPrintText(iFile, out, HashTable, in_size);
		PrintCode(out, 0, 8);
		FreeAll(HashTable, HashTree, out, data);

	}
}



int DecodeNumOfSymb(FILE* iFile) {
	int res = 0;
	for (int i = 0; i < 4; i++) {
		unsigned char tmp = (unsigned char)fgetc(iFile);
		res <<= 8;
		res += (int)tmp;
	}

	return res;
}

int GetBit(bitStream* in) {
	int res;
	if (in->counter == 0) {
		fscanf(in->file, "%c", &in->data);
		in->counter = 8;
	}
		

	if ((in->data & 0x80) == 0)
		res = 0;
	else
		res = 1;

	in->counter--;
	in->data <<= 1;
	return res;
}

unsigned char GetCode(bitStream* in) {
	unsigned char code = 0;
	for (int i = 7; i >= 0; i--) {
		int bit = GetBit(in);
		bit <<= i;
		code += (unsigned char)bit;
	}
	return code;
}

void DecodeHashTree(tree** HashTree, bitStream* in) {
	int bit = GetBit(in);
	if (bit == 0) {
		if (!(*HashTree))
			*HashTree = CreateTree(' ');

		DecodeHashTree(&((*HashTree)->left), in);

		DecodeHashTree(&((*HashTree)->right), in);
		return;
	}

	if (bit == 1) {
		unsigned char code = GetCode(in);
		*HashTree = CreateTree(code);
		return;
	}

	return;
}

void DecodeAndPrintText(bitStream* in, FILE* oFile, tree* HashTree, int in_size) {
	if (!HashTree->left && !HashTree->right) {
		for (int i = 0; i < in_size; i++)
			fprintf(oFile, "%c", HashTree->c);
		return;
	}

	tree* currTree = HashTree;
	while (1) {
		if (feof(in->file) && in->counter == 0)
			return;
		if (in_size == 0)
			return;
		int bit = GetBit(in);
		
		if (!currTree)
			currTree = HashTree;

		if (!currTree->left && !currTree->right) {
			fprintf(oFile, "%c", currTree->c);
			currTree = HashTree;
			in_size--;
		}

		if (bit == 0)
			currTree = currTree->left;
		if (bit == 1)
			currTree = currTree->right;

	}

}

void decode(FILE* iFile, FILE* oFile) {
	unsigned char compressed = (unsigned char)fgetc(iFile);
	if (compressed == 0)
		while (1) {
			int symb = fgetc(iFile);
			if (feof(iFile))
				return;
			fprintf(oFile, "%c", symb);
		}

	else {

		int in_size = DecodeNumOfSymb(iFile);
		if (in_size == 0)
			return;
		bitStream* in = CreateBitStream(iFile);
		tree* HashTree = NULL;
		DecodeHashTree(&HashTree, in);

		if (!HashTree->left && !HashTree->right) {
			for (int i = 0; i <in_size; ++i) {
				fprintf(oFile, "%c", HashTree->c);
			}
			return;
		}
		DecodeAndPrintText(in, oFile, HashTree, in_size);
		free(in);
		FreeTree(&HashTree);
	}
}

int main()
{
	FILE* iFile = fopen("in.txt", "rb");
	FILE* oFile = fopen("out.txt", "wb");
	
	unsigned char c = (unsigned char)fgetc(iFile);
	fseek(iFile, 2, SEEK_CUR);

	if (c == 'c')
		encode(iFile, oFile);
	else
		decode(iFile, oFile);

	fclose(iFile);
	fclose(oFile);
    return 0;
}

