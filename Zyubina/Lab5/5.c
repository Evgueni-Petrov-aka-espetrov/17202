#include <stdio.h>
#include <stdlib.h>

typedef struct bstream {
	FILE* file;
	int c;
	unsigned char data;
} bstream;

typedef struct stack {
	int length;
	int code;
	stack* next;
	unsigned char c;
} stack;

typedef struct tree {
	int freq;
	int length;
	int code;
	struct tree* left;
	struct tree* right;
	unsigned char c;
} tree;

typedef struct symbol {
	int freq;
	unsigned char c;
} symbol;



bstream* make_bstream(FILE* out) {
	bstream* res = (bstream*)malloc(sizeof(bstream));
	res->file = out;
	res->c = 0;
	res->data = 0;
	return res;
}

stack* make_table(int code, int Length, unsigned char c) {
	stack* res = (stack*)malloc(sizeof(stack));
	res->c = c;
	res->length = Length;
	res->code = code;
	res->next = NULL;
	return res;
}


tree* make_tree(unsigned char c) {
	tree* res = (tree*)malloc(sizeof(tree));
	res->left = NULL;
	res->right = NULL;
	res->freq = 0;
	res->length = 0;
	res->code = 0;
	res->c = c;
	return res;
}

symbol* symbols() {
	symbol* res = (symbol*)malloc(sizeof(symbol)* 256);
	for (int i = 0; i < 256; i++) {
		res[i].freq = 0;
		res[i].c = (unsigned char)i;
	}
	return res;
}

tree** forest(symbol* data, int num) {
	tree** res = (tree**)malloc(sizeof(tree*)* num);
	int c = 0;
	for (int i = 0; i < 256; i++) {
		if (data[i].freq != 0) {
			res[c] = make_tree(data[i].c);
			res[c]->freq = data[i].freq;
			c++;
		}
	}
	return res;
}

tree* MatchTrees(tree* first, tree* second) {
	tree* r = make_tree(' ');
	r->freq = first->freq + second->freq;
	r->left = first;
	r->right = second;
	return r;
}


void swap(tree** i, tree** j) {
	tree* tmp = *i;
	*i = *j;
	*j = tmp;
}

void forest_sort(tree** forest, int begin, int end) {
	for (int i = begin; i < end; i++)
	for (int j = i + 1; j < end; j++)
	if (forest[i]->freq > forest[j]->freq)
		swap(&forest[i], &forest[j]);
}

tree* make_H_tree(symbol* data, int num) {
	tree** forest1 = forest(data, num);
	for (int i = 0; i < num - 1; i++) {
		forest_sort(forest1, i, num);
		forest1[i + 1] = MatchTrees(forest1[i], forest1[i + 1]);
	}
	return forest1[num - 1];
}


void push_table(stack** table, unsigned char c, int length, int code) {
	if (!(*table))
		make_table(code, length, c);

	stack* res = make_table(code, length, c);
	res->next = *table;
	*table = res;
}

void make_H_table(tree* HashTree, stack** table) {
	if (!HashTree)
		return;

	if (HashTree->left)
		make_H_table(HashTree->left, table);

	if (HashTree->right)
		make_H_table(HashTree->right, table);

	if (!HashTree->right && !HashTree->left)
		push_table(table, HashTree->c, HashTree->length, HashTree->code);

}

void print_code(bstream* out, int code, int codeLength) {
	for (int i = 0; i < codeLength; i++) {
		if (out->c == 8) {
			fprintf(out->file, "%c", out->data);
			out->c = 0;
			out->data = 0;
		}
		int tmp = code >> (codeLength - 1 - i);
		code = code - (tmp << (codeLength - 1 - i));
		out->data <<= 1;
		out->data += (unsigned char)tmp;
		out->c += 1;
	}
}

void encode_tree(bstream* out, tree* HashTree) {
	if (!HashTree)
		return;

	if (!HashTree->left && !HashTree->right) {
		print_code(out, 1, 1);
		print_code(out, HashTree->c, 8);
		return;
	}

	if (HashTree->left) {
		print_code(out, 0, 1);
		encode_tree(out, HashTree->left);
	}
	if (HashTree->right)
		encode_tree(out, HashTree->right);

	return;
}

void encode_text(FILE* in, bstream* out, stack* HashTable, int in_size) {
	for (int i = 0; i < in_size; i++) {
		unsigned char currSymb = (unsigned char)fgetc(in);
		stack* currTable = NULL;
		stack* temp = HashTable;
		while (temp)
		{
			if (temp->c == currSymb)
			{
				currTable = temp;
				break;
			};
			temp = temp->next;
		};

		print_code(out, currTable->code, currTable->length);
	}
}


int Number(symbol* data) {
	int res = 0;
	for (int i = 0; i < 256; i++)
	if (data[i].freq != 0)
		res++;
	return res;
}

void code_tree(tree* tree) {
	if (!tree)
		return;

	if (tree->left) {
		tree->left->length = tree->length + 1;
		tree->left->code = (tree->code << 1);
		code_tree(tree->left);
	}
	if (tree->right) {
		tree->right->length = tree->length + 1;
		tree->right->code = (tree->code << 1) + 1;
		code_tree(tree->right);
	}
}

symbol* count_symb(FILE* in, int* in_size) {
	symbol* res = symbols();
	while (1) {
		unsigned char c = (unsigned char)fgetc(in);
		if (feof(in))
			break;
		res[(int)c].freq++;
		(*in_size)++;
	}

	fseek(in, 3, SEEK_SET);
	return res;
}

void free_tree(tree** tree) {
	if (!(*tree))
		return;
	if ((*tree)->left)
		free_tree(&(*tree)->left);

	if ((*tree)->right)
		free_tree(&(*tree)->right);

	free(*tree);
}

void free_stack(stack** table) {
	while (*table) {
		stack* tmp = (*table)->next;
		free(*table);
		*table = tmp;
	}
	table = NULL;
}

int compress(symbol* data, int num) {
	if (num == 256) {
		int a = data[0].freq;
		for (int i = 0; i < num; i++)
		if (a != data[i].freq)
			return 1;
		return 0;
	}
	return 1;
}

void print_symb(FILE* out, int in_size) {
unsigned char res[4];
for (int i = 0; i < 4; i++) {
res[i] = in_size & 0xff;
in_size >>= 8;
}
fprintf(out, "%c%c%c%c", res[3], res[2], res[1], res[0]);
}


void encode(FILE* in, FILE* out) {
	int in_size = 0;
	symbol* data = count_symb(in, &in_size);

	int num = Number(data);
	fprintf(out, "%c", compress(data, num));
	if (!compress(data, num))
	while (1) {
		int symb = fgetc(in);
		if (feof(in))
			return;

		fprintf(out, "%c", symb);
	}
	else {
		print_symb(out, in_size);

		if (num == 0)
			return;

		bstream* out1 = make_bstream(out);
		tree* HashTree = make_H_tree(data, num);

		encode_tree(out1, HashTree);

		if (!HashTree->left && !HashTree->right) {
			print_code(out1, 0, 8);
			return;
		}

		code_tree(HashTree);
		stack* HashTable = NULL;
		make_H_table(HashTree, &HashTable);

		encode_text(in, out1, HashTable, in_size);
		print_code(out1, 0, 8);
		free(out1);
		free(data);
		free_tree(&HashTree);
		free_stack(&HashTable);

	}
}

int num_symb(FILE* in) {
	int ans = 0;
	for (int i = 0; i < 4; i++) {
		unsigned char tmp = (unsigned char)fgetc(in);
		ans <<= 8;
		ans += (int)tmp;
	}

	return ans;
}

int Bit(bstream* inn) {
	int ans;
	if (inn->c == 0) {
		fscanf(inn->file, "%c", &inn->data);
		inn->c = 8;
	}


	if ((inn->data & 0x80) == 0)
		ans = 0;
	else
		ans = 1;

	inn->c--;
	inn->data <<= 1;
	return ans;
}

void decode_H_tree(tree** HashTree, bstream* inn) {
	int bit = Bit(inn);
	if (bit == 0) {
		if (!(*HashTree))
			*HashTree = make_tree(' ');

		decode_H_tree(&((*HashTree)->left), inn);

		decode_H_tree(&((*HashTree)->right), inn);
		return;
	}

	if (bit == 1) {
		unsigned char code = 0;
		for (int i = 7; i >= 0; i--) {
			int bit = Bit(inn);
			bit <<= i;
			code += (unsigned char)bit;
		}
		*HashTree = make_tree(code);
		return;
	}

	return;
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

void decode_text(bstream* inn, FILE* out, tree* HashTree, int in_size) {
	if (!HashTree->left && !HashTree->right) {
		for (int i = 0; i < in_size; i++)
			fprintf(out, "%c", HashTree->c);
		return;
	}

	tree* currTree = HashTree;
	while (1) {
		if (feof(inn->file) && inn->c == 0)
			return;
		if (in_size == 0)
			return;
		int bit = Bit(inn);

		if (!currTree)
			currTree = HashTree;

		if (!currTree->left && !currTree->right) {
			fprintf(out, "%c", currTree->c);
			currTree = HashTree;
			in_size--;
		}

		if (bit == 0)
			currTree = currTree->left;
		if (bit == 1)
			currTree = currTree->right;

	}

}

void decode(FILE* in, FILE* out) {
	unsigned char compressed = (unsigned char)fgetc(in);
	if (compressed == 0)
	while (1) {
		int symb = fgetc(in);
		if (feof(in))
			return;
		fprintf(out, "%c", symb);
	}

	else {

		int numer = num_symb(in);
		if (numer == 0)
			return;
		bstream* inn = make_bstream(in);
		tree* HashTree = NULL;
		decode_H_tree(&HashTree, inn);

		if (!HashTree->left && !HashTree->right) {
			for (int i = 0; i < numer; ++i) {
				fprintf(out, "%c", HashTree->c);
			}
			return;
		}
		decode_text(inn, out, HashTree, numer);
		free(inn);
		FreeTree(&HashTree);
	}
}

int main()
{
	FILE* in = fopen("in.txt", "rb");
	FILE* out = fopen("out.txt", "wb");

	unsigned char c = (unsigned char)fgetc(in);
	fseek(in, 2, SEEK_CUR);

	if (c == 'c')
		encode(in, out);
	if (d == 'd')
		decode(in, out);

	fclose(in);
	fclose(out);
	return 0;
}
