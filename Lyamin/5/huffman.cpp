#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

enum { MAX_SIZE = 256, CODE_SIZE = 8 };

using namespace std;

struct tree {
	int num;
	int code_length;
	unsigned char c;
	unsigned char* code;
	struct tree* left;
	struct tree* right;
};
struct symbol {
	int sum;
	unsigned char c;
};
struct table {
	int code_length;
	unsigned char c;
	unsigned char* code;
	table* next;
};
struct bit_stream {
	FILE* stream;
	int count;
	unsigned char data[8];
};

table* table_ctor(unsigned char*, unsigned char, int);
void table_push(table**, unsigned char*, unsigned char, int);
table* table_find(table*, unsigned char);
void table_build(tree*, table*);

tree* tree_build(FILE*);
void tree_encode(bit_stream*, tree*);
void tree_decode(bit_stream*, tree**);
void tree_coding(tree*);
tree* tree_ctor(char);
tree* tree_dtor(tree*);
void tree_score_code(tree*, int*);

symbol* symbol_ctor();
symbol* symbol_seq_ctor(int);
void symbol_push(symbol*, unsigned char);
symbol* symbol_read_for_coding(FILE*);
int symbol_num_of_not_zero(symbol*);
int symbol_read(FILE*, symbol**);
void forest_swap(tree**, int, int);
void forest_sort(tree**, int, int);
void tree_coll(tree**, int, int);
tree** make_forest(symbol*, int);

bit_stream* bit_stream_ctor(FILE*);
unsigned char get_bit(bit_stream*);

void print_code(bit_stream*, unsigned char*, int);
void printing_code(table*, tree*, unsigned int);
unsigned char get_bit(bit_stream*);

unsigned char unsigned_char_code_to_unsigned_char(unsigned char[8]);
unsigned char* unsigned_char_to_code_of_unsigned_char(unsigned char);

void decode(FILE*);
void encode(FILE*);

//---------------------------table------------------------------------//
void table_build(tree* h_tree, table** h_table) {
	if (!h_tree) {	return; }
	else {
		if (h_tree->left)
			table_build(h_tree->left, h_table);
		if (h_tree->right)
			table_build(h_tree->right, h_table);
		if (!h_tree->left && !h_tree->right)
			table_push(h_table, h_tree->code, h_tree->c, h_tree->code_length);
	}
}
table* table_ctor(unsigned char* code, unsigned char c, int code_length) {
	table* result = (table*)malloc(sizeof(table));
	result->next = NULL;
	result->c = c;
	result->code = code;
	result->code_length = code_length;
	return result;
}
void table_push(table** in, unsigned char* code, unsigned char c, int code_length) {
	if (!(*in)) {
		*in = table_ctor(code, c, code_length);
	}
	table* result = table_ctor(code, c, code_length);
	result->next = *in;
	*in = result;
}
table* table_find(table* h_table, unsigned char c) {
	table* result = h_table;
	while (result) {
		if (result->c == c) {
			return result;
		}
		result = result->next;
	}
	return NULL;
}
//---------------------------table------------------------------------//

//---------------------------symbol------------------------------------//
symbol* symbol_ctor() {
	symbol* result = (symbol*)malloc(sizeof(symbol));
	result->sum = 0;
	return result;
}
void symbol_dtor(symbol** in) {
	free(*in);
	return;
}
symbol* symbol_seq_ctor(int num) {
	symbol* result = (symbol*)malloc(sizeof(symbol)*num);
	for (int i = 0; i < num; ++i) {
		result[i].c = (unsigned char)i;
		result[i].sum = 0;
	}
	return result;
}
void symbol_push(symbol* in, unsigned char c) {
	in[int(c)].sum++;
}
symbol* symbol_read_for_coding(FILE* input, unsigned int* in_size) {
	symbol* result = symbol_seq_ctor(MAX_SIZE);
	while (true) {
		unsigned char c = fgetc(input);
		if (feof(input)) { break; }
		++*in_size;
		symbol_push(result, c);
	}
	fclose(input);
	return result;
}
int symbol_num_of_not_zero(symbol* in) {
	int num = 0;
	for (int i = 0; i < MAX_SIZE; ++i) {
		if (in[i].sum != 0) {
			++num;
		}
	}
	return num;
}
int symbol_read(FILE* input, symbol** result, unsigned int* in_size) {
	symbol* symbol_cin = symbol_read_for_coding(input, in_size);
	int num = symbol_num_of_not_zero(symbol_cin);
	if (num == 0) { *result = NULL; }
	else { *result = symbol_seq_ctor(num); }
	int flag = 0;
	for (int i = 0; i < MAX_SIZE; ++i) {
		if (symbol_cin[i].sum != 0) {
			(*result)[flag] = symbol_cin[i];
			++flag;
		}
	}
	return num;
}
//---------------------------symbol------------------------------------//

//---------------------------tree------------------------------------//
tree* tree_ctor(char c) {
	tree* result = (tree*)malloc(sizeof(tree));
	result->left = NULL;
	result->right = NULL;
	result->code = NULL;
	result->num = 0;
	result->code_length = 0;
	result->c = c;
	return result;
}
void tree_dtor(tree** t) {
	if (!(*t)) {
		return;
	}
	tree_dtor(&((*t)->left));
	tree_dtor(&((*t)->right));
	free(*t);
	return;
}
tree** make_forest(symbol* in, int num) {
	tree** forest = (tree**)malloc(sizeof(tree*)*num);
	for (int i = 0; i < num; ++i) {
		forest[i] = tree_ctor(in[i].c);
		forest[i]->num = in[i].sum;
	}
	return forest;
}
tree* tree_build(FILE* input, unsigned int* in_size) {
	symbol* in = NULL;
	int num = symbol_read(input, &in, in_size);
	if (in == NULL) { 
		return NULL;
	}
	tree** forest = make_forest(in, num);
	symbol_dtor(&in);
	forest_sort(forest, 0, num);
	int begin = 0;
	while (begin != num - 1) {
		tree_coll(forest, begin, begin + 1);
		++begin;
		forest_sort(forest, begin, num);
	}
	return forest[num - 1];
}
void tree_coding(tree* t) {
	if (!t) {
		return;
	}
	if (t->left) {
		t->left->code_length = t->code_length + 1;
		t->left->code = (unsigned char*)malloc(sizeof(unsigned char) * (t->left->code_length));
		for (int i = 0; i < t->code_length; ++i) {
			t->left->code[i] = t->code[i];
		}
		t->left->code[t->left->code_length - 1] = '0';

		tree_coding(t->left);
	}

	if (t->right) {
		t->right->code_length = t->code_length + 1;
		t->right->code = (unsigned char*)malloc(sizeof(unsigned char) * (t->right->code_length));
		for (int i = 0; i < t->code_length; ++i) {
			t->right->code[i] = t->code[i];
		}
		t->right->code[t->right->code_length - 1] = '1';
		tree_coding(t->right);
	}
	return;
}
void tree_score_code(tree* h_tree, int* k) {
	if (!h_tree) {
		return;
	}
	if (!h_tree->left && !h_tree->right) {
		*k += h_tree->code_length + 1;
		return;
	}
	else {
		if (h_tree->left) {
			tree_score_code(h_tree->left, k);
		}
		if (h_tree->right) {
			tree_score_code(h_tree->right, k);
		}
	}
	return;

}
void tree_encode(bit_stream* output, tree* h_tree) {
	unsigned char zero = '0';
	unsigned char one = '1';
	if (!h_tree) {
		return;
	}
	if (!h_tree->left && !h_tree->right) {
		print_code(output, &one, 1);
		print_code(output, unsigned_char_to_code_of_unsigned_char(h_tree->c), 8);
		
	}
	else {
		if (h_tree->left) {
			print_code(output, &zero, 1);
			tree_encode(output, h_tree->left);
		}
		if (h_tree->right) {
			tree_encode(output, h_tree->right);
		}
	}
	return;
}
void tree_decode(bit_stream* input, tree** h_tree) {
	unsigned char c = get_bit(input);
	if (feof(input->stream)) { return;}
	if (c == '0') {
		if (!(*h_tree)) {
			*h_tree = tree_ctor(' ');
		}
		tree_decode(input, &((*h_tree)->left));
		tree_decode(input, &((*h_tree)->right));
		return;
	}
	if (c == '1') {
		unsigned char* code = (unsigned char*)malloc(sizeof(int) * 8);
		for (int i = 0; i < 8; ++i) {
			code[i] = get_bit(input);
		}
		if (!(*h_tree)) {
			*h_tree = tree_ctor(unsigned_char_code_to_unsigned_char(code));
		}
		return;
	}
	return;
}
void forest_swap(tree** forest, int i, int j) {
	tree* buff = forest[i];
	forest[i] = forest[j];
	forest[j] = buff;
	return;
}
void forest_sort(tree** forest, int begin, int end) {
	for (int i = begin; i < end; ++i) {
		for (int j = i + 1; j < end; ++j) {
			if (forest[i]->num > forest[j]->num)
				forest_swap(forest, i, j);
		}
	}
	return;
}
void tree_coll(tree** forest, int begin, int n) {
	tree* buff = tree_ctor(' ');
	buff->num = forest[begin]->num + forest[begin + 1]->num;
	buff->left = forest[begin];
	buff->right = forest[begin + 1];
	forest[begin + 1] = buff;
	return;
}
//---------------------------tree------------------------------------//

//---------------------------printing------------------------------------//
void text_encode(table* h_table, FILE* input, bit_stream* out) {
	while (true) {
		unsigned char c = fgetc(input);
		if (feof(input)) {
			break;
		}
		table* ht_c = table_find(h_table, c);
		if (ht_c == NULL) { break; }
		print_code(out, ht_c->code, ht_c->code_length);
	}
	fclose(input);
	return;
}
void print_code(bit_stream* output, unsigned char* code, int code_length) {
	for (int i = 0; i < code_length; ++i) {
		output->data[(i + output->count) % 8] = code[i];
		if (i + output->count == 7) {
			fprintf(output->stream, "%c", unsigned_char_code_to_unsigned_char(output->data));
			memset(output->data, '0', 8 * sizeof(unsigned char));
		}
	}
	output->count = (code_length + output->count) % 8;
	return;
}
void print_num_of_char(bit_stream* out, unsigned int in_size) {
	//cout << endl;
	//cout << in_size << endl;
	unsigned int fourth = in_size % 256;
	in_size >>= 8;
	unsigned int third = in_size % 256;
	in_size >>= 8;
	unsigned int second = in_size % 256;
	in_size >>= 8;
	unsigned int first = in_size % 256;
	print_code(out, unsigned_char_to_code_of_unsigned_char(first), 8);
	print_code(out, unsigned_char_to_code_of_unsigned_char(second), 8);
	print_code(out, unsigned_char_to_code_of_unsigned_char(third), 8);
	print_code(out, unsigned_char_to_code_of_unsigned_char(fourth), 8);
	return;
}
unsigned int num_of_char(bit_stream* in) {
	unsigned int result = 0;
	for (int i = 0; i < 4; ++i) {
		result <<= 8;
		unsigned char* num_code = (unsigned char*)malloc(sizeof(unsigned char) * 8);
		for (int i = 0; i < 8; ++i) {
			num_code[i] = get_bit(in);
		}
		result += (unsigned int)unsigned_char_code_to_unsigned_char(num_code);
	}
	return result;
}
void printing_code(table* h_table, tree* h_tree, unsigned int in_size) {
	FILE* output = fopen("out.txt", "wb");
	bit_stream* out = bit_stream_ctor(output);
	unsigned char zero = '0';
	unsigned char one = '1';
	tree_encode(out, h_tree);
	int tree_rest = (8 - out->count) % 8;
	for (int i = 0; i < tree_rest; ++i) {
		print_code(out, &zero, 1);
	}
	print_num_of_char(out, in_size);
	tree_dtor(&h_tree);
	FILE* read_again = fopen("in.txt", "rb");
	unsigned char garbige = fgetc(read_again);
	garbige = fgetc(read_again);
	garbige = fgetc(read_again);
	text_encode(h_table, read_again, out);
	int code_rest = (8 - out->count) % 8;

	for (int i = 0; i < code_rest; ++i) {
		print_code(out, &zero, 1);
	}
	fclose(out->stream);
	
	return;
}
//---------------------------printing------------------------------------//

//---------------------------unsigned-char-------------------------------//
unsigned char unsigned_char_code_to_unsigned_char(unsigned char code[8]) {
	unsigned char result = (code[0] - '0') * 128 + (code[1] - '0') * 64 
						 + (code[2] - '0') * 32  + (code[3] - '0') * 16
						 + (code[4] - '0') * 8   + (code[5] - '0') * 4 
						 + (code[6] - '0') * 2   + (code[7] - '0');
	return result;
}
unsigned char* unsigned_char_to_code_of_unsigned_char(unsigned char c) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char) * 8);
	for (int i = 7; i >= 0; --i) {
		result[i] = (c & 0x01);
		result[i] += '0';
		c = c >> 1;
	}
	return result;
}
//---------------------------unsigned-char-------------------------------//

//---------------------------others--------------------------------------//
bit_stream* bit_stream_ctor(FILE* stream) {
	bit_stream* result = (bit_stream*)malloc(sizeof(bit_stream));
	result->stream = stream;
	result->count = 0;
	memset(result->data, 0, 8 * sizeof(unsigned char));
	return result;
}
unsigned char get_bit(bit_stream* input) {
	if (input->count == 0) {
		unsigned char c = fgetc(input->stream);
		unsigned char* code = unsigned_char_to_code_of_unsigned_char(c);
		for (int i = 0; i < 8; ++i) {
			input->data[i] = code[i];
		}
		input->count = 8;
	}
	unsigned char result = input->data[0];
	for (int i = 0; i < input->count; ++i) {
		input->data[i] = input->data[i + 1];
	}
	--input->count;
	return result;
}

void text_decode(bit_stream* in, FILE* output, tree* h_tree, int in_num) {
	tree* t_iter = h_tree;
	unsigned char c;
	if (!h_tree) {
		fprintf(output, "error\n");
		return;
	}
	while (true) {
		c = get_bit(in);
		if (feof(in->stream) && in->count == 0) { break; }
		if (!t_iter) { t_iter = h_tree; }

		if (!t_iter->left && !t_iter->right) {

			if (in_num == 0) { break; }
			fprintf(output, "%c", t_iter->c);
			t_iter = h_tree;
			--in_num;
		}
		if (c == '0') {
			t_iter = t_iter->left;
			continue;
		}
		if (c == '1') {
			t_iter = t_iter->right;
			continue;
		}
	}
}

void decode(FILE* input) {
	FILE* output = fopen("out.txt", "wb");
	bit_stream* in = bit_stream_ctor(input);
	tree* h_tree = NULL;
	tree_decode(in, &h_tree);
	if (!h_tree) {	return; }
	tree_coding(h_tree);
	int rest = in->count;
	for (int i = 0; i < rest; ++i) {
		unsigned char c = get_bit(in);
	}
	int num = num_of_char(in);
	text_decode(in, output, h_tree, num);
	return;
}
void encode(FILE* input) {
	unsigned int in_size = 0;
	tree* h_tree = tree_build(input, &in_size);
	if (!h_tree) {	return; }

	if (!h_tree->left && !h_tree->right) {
		unsigned char* zero = (unsigned char*)malloc(sizeof(unsigned char)*1);
		zero[0] = '0';
		h_tree->code = zero;
		h_tree->code_length = 1;
	}
	else {
		tree_coding(h_tree);
	}
	table* h_table = NULL;
	table_build(h_tree, &h_table);
	printing_code(h_table, h_tree, in_size);
	return;
}
//---------------------------others--------------------------------------//

int main() {
	FILE* input = fopen("in.txt", "rb");
	unsigned char c = fgetc(input);
	switch (c) {
		case 'c': {
			//cout << "encode" << endl;
			c = fgetc(input);
			c = fgetc(input);
			//cout << (int)c << endl;
			encode(input);
			break;
		}
		case 'd': {
			c = fgetc(input);
			c = fgetc(input);
			decode(input);
			break;
		}
	}
	return 0;
}
