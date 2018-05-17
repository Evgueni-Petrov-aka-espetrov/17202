#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

enum{ MAX_SIZE = 256 };

struct bit_stream {
	FILE* stream;
	int count;
	unsigned char data;
};
struct tree {
	int frequency;
	unsigned int code_length;
	unsigned int code;
	struct tree* left;
	struct tree* right;
	unsigned char c;
};
struct symbol {
	int frequency;
	unsigned char c;
};
struct table {
	int code_length;
	unsigned int code;
	table* next;
	unsigned char c;
};

bit_stream* bit_stream_ctor(FILE* stream) {
	bit_stream* result = (bit_stream*)malloc(sizeof(bit_stream));
	result->stream = stream;
	result->count = 0;
	result->data = 0;
	return result;
}
table* table_ctor(unsigned int code, unsigned char c, unsigned int code_length) {
	table* result = (table*)malloc(sizeof(table));
	result->c = c;
	result->code = code;
	result->code_length = code_length;
	result->next = NULL;
	return result;
}
void table_dtor(table** h_table) {
	table* tmp = *h_table;
	while (tmp->next) {
		table* buff = tmp->next;
		free(tmp);
		tmp = buff;
	}
	free(tmp);
	free(h_table);
	return;
}
void table_push(table** in, unsigned int code, unsigned char c, unsigned int code_length) {
	if (!(*in)) { *in = table_ctor(code, c, code_length); }
	table* result = table_ctor(code, c, code_length);
	result->next = *in;
	*in = result;
}
void table_build(tree* h_tree, table** h_table) {
	if (!h_tree) {
		return;
	}
	else {
		if (h_tree->left)
			table_build(h_tree->left, h_table);

		if (h_tree->right)
			table_build(h_tree->right, h_table);

		if (!h_tree->left && !h_tree->right)
			table_push(h_table, h_tree->code, h_tree->c, h_tree->code_length);
	}
}

void print_code(bit_stream* output, unsigned int code, unsigned int code_length) {
	for (int i = 0; i < code_length; ++i) {
		if (output->count == 8) {
			fprintf(output->stream, "%c", output->data);
			output->count = 0;
			output->data = 0;
		}
		int tmp = code >> (code_length - 1 - i);
		code = code - (tmp << (code_length - 1 - i));
		output->data <<= 1;
		output->data += tmp;
		++output->count;
	}
	return;
}
int get_bit(bit_stream* input) {
	if (input->count == 0) {
		fscanf(input->stream, "%c", &input->data);
		input->count = 8;
	}
	int result;
	if ((input->data & 0x80) == 0) { result = 0; }
	else { result = 1; }
	--input->count;
	input->data <<= 1;
	
	return result;
}

symbol* symbol_seq_ctor(int size) {
	symbol* result = (symbol*)malloc(sizeof(symbol) * size);
	for (int i = 0; i < MAX_SIZE; ++i) {
		result[i].frequency = 0;
		result[i].c = (unsigned char)i;
	}
	return result;
}
void symbol_increase(symbol* in, unsigned char c) {
	++in[int(c)].frequency;
	return;
}
symbol* symbol_reading(FILE* input, unsigned int* in_size) {
	symbol* result = symbol_seq_ctor(MAX_SIZE);
	//FILE* prev_test = fopen("test.txt", "w");
	while (true) {
		
		unsigned char tmp = fgetc(input);
		if (feof(input)) { break; }
		//fprintf(prev_test, "%c", tmp);
		++*in_size;
		symbol_increase(result, tmp);
	}
	//fclose(input);
	fseek(input, 3, SEEK_SET);
	return result;
}
int symbol_num_of_non_empty(symbol* in) {
	int result = 0;
	for (int i = 0; i < MAX_SIZE; ++i)
		if (in[i].frequency != 0)
			++result;
	return result;
}

tree* tree_ctor(char c) {
	tree* result = (tree*)malloc(sizeof(tree));
	result->left = NULL;
	result->right = NULL;
	result->frequency = 0;
	result->code_length = 0;
	result->code = 0;
	result->c = c;
	return result;
}
void tree_dtor(tree** h_tree) {
	if (!(*h_tree)) { return; }
	tree_dtor(&((*h_tree)->left));
	tree_dtor(&((*h_tree)->right));
	tree_dtor(h_tree);
	return;
}
void forest_swap(tree** forest, int i, int j) {
	tree* buff = forest[i];
	forest[i] = forest[j];
	forest[j] = buff;
	return;
}
void forest_sort(tree** forest, int begin, int end) {
	for (int i = begin; i < end; ++i)
		for (int j = i + 1; j < end; ++j)
			if (forest[i]->frequency > forest[j]->frequency)
				forest_swap(forest, i, j);
	return;
}
void tree_coll(tree** forest, int begin) {
	tree* buff = tree_ctor(' ');
	buff->frequency = forest[begin]->frequency + forest[begin + 1]->frequency;
	buff->left = forest[begin];
	buff->right = forest[begin + 1];
	forest[begin + 1] = buff;
	return;
}
tree** make_forest(symbol* in) {
	int size = symbol_num_of_non_empty(in);
	tree** forest = (tree**)malloc(sizeof(tree*) * size);
	int counter = 0;
	for (int i = 0; i < MAX_SIZE; ++i) {
		if(in[i].frequency != 0) {
			forest[counter] = tree_ctor(in[i].c);
			forest[counter]->frequency = in[i].frequency;
			++counter;
		}
	}
	return forest;
}
tree* tree_build(symbol* in, int num) {
	tree** forest = make_forest(in);
	int begin = 0;
	while (begin != num - 1) {
		forest_sort(forest, begin, num);
		tree_coll(forest, begin);
		++begin;
	}
	return forest[num - 1];
}
void tree_coding(tree* t) {
	if (!t) { return; }
	if (t->left) {
		t->left->code_length = t->code_length + 1;
		t->left->code = (t->code << 1);
		tree_coding(t->left);
	}
	if (t->right) {
		t->right->code_length = t->code_length + 1;
		t->right->code = (t->code << 1) + 1;
		tree_coding(t->right);
	}
	return;
}
void tree_encode(bit_stream* output, tree* h_tree) {
	if (!h_tree) { return; }
	if (!h_tree->left && !h_tree->right) {
		print_code(output, 1, 1);
		print_code(output, h_tree->c, 8);
		return;
	}
	else {
		if (h_tree->left) {
			print_code(output, 0, 1);
			tree_encode(output, h_tree->left);
		}
		if (h_tree->right) {
			tree_encode(output, h_tree->right);
		}
		return;
	}
	return;
}
void tree_decode(bit_stream* input, tree** h_tree) {
	int c = get_bit(input);
	if (c == 0) {
		if (!(*h_tree)) {
			*h_tree = tree_ctor(' ');
		}
		tree_decode(input, &((*h_tree)->left));
		tree_decode(input, &((*h_tree)->right));
		return;
	}
	if (c == 1) {
		unsigned char code = 0;
		for (int i = 7; i >= 0; --i) {
			int tmp = get_bit(input);
			tmp <<= i;
			code += tmp;
		}

		*h_tree = tree_ctor(code);
		return;
	}
	return;
}

void print_tree(const tree* h_tree) {
	if (h_tree == NULL) {
		return;
	}
	if (h_tree->left == NULL && h_tree->right == NULL) {
		fprintf(stderr, "0x%02x", h_tree->c);
		return;
	}
	fprintf(stderr, "[");
	print_tree(h_tree->left);
	fprintf(stderr, " ");
	print_tree(h_tree->right);
	fprintf(stderr, "]");
}
void print_table(table* h_table) {
	fprintf(stderr, "TABLE:\n");
	while (h_table) {
		fprintf(stderr, "%d, %d\n", h_table->code, h_table->code_length);
		h_table = h_table->next;
	}
	fprintf(stderr, "end\n\n");
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
void text_encode(FILE* input, bit_stream* output, table* h_table, int num) {
	for (int i = 0; i < num; ++i) {
		unsigned char tmp = fgetc(input);
		table* ht_c = table_find(h_table, tmp);
		if (ht_c == NULL) { fprintf(stderr, "table not found"); return; }
		print_code(output, ht_c->code, ht_c->code_length);
	}
	return;
}
void text_decode(bit_stream* input, FILE* output, tree* h_tree, int num) {
	if (!h_tree) {
		fprintf(stderr, "text_decode_error_1");
		return;
	}
	if (!h_tree->left && !h_tree->right) {
		for (int i = 0; i < num; ++i) {
			fprintf(output, "%c", h_tree->c);
		}
		return;
	}
	tree* t_iter = h_tree;
	while (true) {
		int tmp = get_bit(input);
		if (feof(input->stream) && input->count == 0) { return; }
		if (!t_iter) { t_iter = h_tree; }
		if (num == 0) { return; }
		if (!t_iter->left && !t_iter->right) {
			fprintf(output, "%c", t_iter->c);
			t_iter = h_tree;
			--num;
		}
		if (tmp == 0) { t_iter = t_iter->left; continue; }
		if (tmp == 1) { t_iter = t_iter->right; continue; }

		fprintf(stderr, "text_decode_error_2");
	}
	return;
}
unsigned char uncompressed(symbol* s, int size) {
	if (size == MAX_SIZE) {
		int freq = s[0].frequency;
		for (int i = 1; i < size; ++i)
			if (s[i].frequency != freq)
				return 0;
		return 1;
	}
	return 0;
}
void print_num_of_chars(FILE* output, unsigned int in_size) {
	unsigned char result[4];
	for (int i = 0; i < 4; ++i) {
		result[i] = in_size & 0xff;
		in_size >>= 8;
	}
	fprintf(output, "%c%c%c%c", result[3], result[2], result[1], result[0]);
	return;
}

void encode(FILE* input) {
	unsigned int in_size = 0;
	FILE* output = fopen("out.txt", "wb");
	symbol* da = symbol_reading(input, &in_size);
	int num = symbol_num_of_non_empty(da);
	unsigned char flag = uncompressed(da, num);
	fprintf(output, "%c", flag);

	switch (flag) {
		case 1: {
			while (1) {
				char tmp = getc(input);
				if (feof(input))
					return;
				fprintf(output, "%c", tmp);
			}
			fclose(output);
			return;
		}
		case 0: {
			bit_stream* out = bit_stream_ctor(output);
			print_num_of_chars(output, in_size);
			if (num == 0) { return; }
			tree* h_tree = tree_build(da, num);
			if (!h_tree) { return; }
			tree_encode(out, h_tree);

			if (!h_tree->left && !h_tree->right) {
				print_code(out, 0, 8);
				return;
			}
			else {
				tree_coding(h_tree);	
				table* h_table = NULL;
				table_build(h_tree, &h_table);
				text_encode(input, out, h_table, in_size);
				print_code(out, 0, 8);
				
			}
			return;
		}
		default: fprintf(stderr, "compressed error");
	}

	return; // make compiller be happy
}

unsigned int num_of_char(FILE* input) {
	unsigned int result = 0;
	for (int i = 0; i < 4; ++i) {
		unsigned char tmp = fgetc(input);
		result <<= 8;
		result += tmp;
	}
	return result;
}

void decode(FILE* input) {
	FILE* output = fopen("out.txt", "wb");
	unsigned char uncompressed;
	fscanf(input, "%c", &uncompressed);

	switch (uncompressed) {
		case 1: {
			while (1) {
				char tmp = getc(input);
				if (feof(input))
					return;
				fprintf(output, "%c", tmp);
			}
		} break;

		case 0: {
			unsigned int num = num_of_char(input);
			if (num == 0) {	return; }
			bit_stream* in = bit_stream_ctor(input);
			tree* h_tree = NULL;
			tree_decode(in, &h_tree);
			if (!h_tree->left && !h_tree->right) {
				for (int i = 0; i < num; ++i) {
					fprintf(output, "%c", h_tree->c);
				}
				return;
			}
			text_decode(in, output, h_tree, num);
		} break;
		default: fprintf(stderr, "scanf_error");
			break;
	}
	return;
}

int main() {
	FILE* input = fopen("in.txt", "rb");
	unsigned char c = fgetc(input);
	fseek(input, 2, SEEK_CUR);

	switch (c) {
		case 'c': {
			encode(input);
		} break;
		case 'd': {
			decode(input);
		} break;
		default: {
			fprintf(stderr, "%c errorrr\n", c);
		} break;
	}
	return 0;
}
