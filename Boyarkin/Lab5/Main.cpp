#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BYTE = 256;
const char* TEMP_FILE_NAME = "saved_ocur.txt";

struct bit_stream {
	FILE *f;
	int data;
	int count;
};

struct binary_tree {
	char val;
	binary_tree *left, *right;
};

struct huffman_list {
	int val, len;
	unsigned char *symbols;

	huffman_list *next;
};

void encode(FILE *in, bit_stream *stream, char **cipher);
void decode(FILE *out, bit_stream *stream, char **cipher, int count);

bit_stream* create_stream(FILE *f);
int get_bit(bit_stream *stream);
void put_bit(bit_stream *stream, char bit);
void put_bits(bit_stream *stream, const char *sequence);
void print_remainder(bit_stream *stream);
void free_stream(bit_stream *stream);

binary_tree* create_branch();
binary_tree* build_tree(char **cipher);
void clear_tree(binary_tree *tree);

huffman_list* create_list();
void add_list(huffman_list **list, huffman_list *obj);
void pop_list(huffman_list **list);
void combine_symbols(huffman_list *dest, huffman_list *lhs, huffman_list *rhs);
void clear_list(huffman_list **list);

int get_symbol_count(const int *occurrences);
int* get_occurrences(FILE *in);
int* read_occurrences(FILE *in);
void save_int_array(const int *a, int n);

char** create_cipher();
char** get_cipher(int *occurrencies);
void free_cipher(char **cipher);


int main() {
	FILE *in, *out;
	in = fopen("in.txt", "rb");
	out = fopen("out.txt", "wb");

	bit_stream *stream;
	int *ocr;
	char **cipher;
	char mode[4];


	fgets(mode, 4, in);

	if (mode[0] == 'c') {
		ocr = get_occurrences(in);
		cipher = get_cipher(ocr);
		stream = create_stream(out);

		fgets(mode, 4, in);
		encode(in, stream, cipher);
		save_int_array(ocr, BYTE);

		free(ocr);
		free_cipher(cipher);
		free_stream(stream);
	}
	else if (mode[0] == 'd') {
		FILE *temp = fopen(TEMP_FILE_NAME, "r");
		ocr = read_occurrences(temp);
		cipher = get_cipher(ocr);
		stream = create_stream(in);

		decode(out, stream, cipher, get_symbol_count(ocr));

		free(ocr);
		free_cipher(cipher);
		free_stream(stream);
		fclose(temp);
	}

	fclose(in);
	fclose(out);
	return 0;
}

void encode(FILE *in, bit_stream *stream, char **cipher) {
	unsigned char c;

	while (true) {
		c = fgetc(in);
		if (feof(in)) break;

		put_bits(stream, cipher[c]);
	}
	print_remainder(stream);
}

void decode(FILE *out, bit_stream *stream, char **cipher, int count) {
	binary_tree *tree = build_tree(cipher);
	binary_tree *path;
	int bit;

	for (int i = 0; i < count; ++i) {
		path = tree;
		while ((path->left != NULL) || (path->right != NULL)) {
			bit = get_bit(stream);
			path = (bit == 0) ? path->left : path->right;
		}
		fputc(path->val, out);
	}
	clear_tree(tree);
}

bit_stream* create_stream(FILE *f) {
	bit_stream *stream = (bit_stream*)malloc(sizeof(bit_stream));
	stream->f = f;
	stream->count = 0;

	return stream;
}

int get_bit(bit_stream *stream) {

	if (stream->count == 0) {
		stream->data = getc(stream->f);
		stream->count = 8;
	}
	--stream->count;

	int result = stream->data & 1;
	stream->data >>= 1;
	return result;
}

void put_bit(bit_stream *stream, char bit) {
	if (bit == '1') {
		stream->data |= (1 << stream->count);
	}
	else if (bit == '0') {
		stream->data &= ~(1 << stream->count);
	}
	else { return; }

	++stream->count;
	if (stream->count == 8) {
		putc(stream->data, stream->f);
		stream->count = 0;
	}
}

void put_bits(bit_stream *stream, const char *sequence) {
	for (const char *c = sequence; *c != '\0'; ++c) {
		put_bit(stream, *c);
	}
}

void print_remainder(bit_stream *stream) {
	if (stream->count > 0) {
		putc(stream->data, stream->f);
		stream->count = 0;
	}
}

void free_stream(bit_stream *stream) {
	free(stream);
}

binary_tree* create_branch() {
	binary_tree *tree = (binary_tree*)malloc(sizeof(binary_tree));
	tree->left = tree->right = NULL;

	return tree;
}

binary_tree* build_tree(char **cipher) {
	binary_tree *tree = create_branch();
	binary_tree *path;

	for (int i = 0; i < BYTE; ++i) {
		path = tree;
		for (char *c = cipher[i]; *c != '\0'; ++c) {
			if (*c == '0') {
				if (path->left == NULL) { path->left = create_branch(); }
				path = path->left;
			}
			else if (*c == '1') {
				if (path->right == NULL) { path->right = create_branch(); }
				path = path->right;
			}
		}
		path->val = (char)i;
	}
	return tree;
}

void clear_tree(binary_tree *tree) {
	if (tree == NULL) { return; }

	clear_tree(tree->left);
	clear_tree(tree->right);
	free(tree);
}

huffman_list* create_list() {
	huffman_list *list = (huffman_list*)malloc(sizeof(huffman_list));
	list->symbols = (unsigned char*)malloc(sizeof(unsigned char) * BYTE);
	list->len = 0;
	list->next = NULL;

	return list;
}

void add_list(huffman_list **list, huffman_list *obj) {
	if (*list == NULL) {
		*list = obj;
		return;
	}
	else if (obj->val <= (*list)->val) {
		obj->next = *list;
		*list = obj;
	}

	huffman_list *prev = *list;
	huffman_list *iter = prev->next;

	for (; iter != NULL; prev = iter, iter = iter->next) {
		if (obj->val <= iter->val) { break; }
	}

	prev->next = obj;
	obj->next = iter;
}

void pop_list(huffman_list **list) {
	if (*list == NULL) { return; }

	huffman_list *temp = (*list)->next;
	free((*list)->symbols);
	free(*list);
	*list = temp;
}

void combine_symbols(huffman_list *dest, huffman_list *lhs, huffman_list *rhs) {
	dest->len = 0;
	for (int i = 0; i < lhs->len; ++i) {
		dest->symbols[dest->len] = lhs->symbols[i];
		++dest->len;
	}
	for (int i = 0; i < rhs->len; ++i) {
		dest->symbols[dest->len] = rhs->symbols[i];
		++dest->len;
	}
}

void clear_list(huffman_list **list) {
	while (*list != NULL) {
		pop_list(list);
	}
}

int get_symbol_count(const int *occurrences) {
	int sum = 0;
	for (int i = 0; i < BYTE; ++i) { sum += occurrences[i]; }
	return sum;
}

int* get_occurrences(FILE *in) {
	int *p = new int[BYTE];
	memset(p, 0, BYTE * sizeof(*p));

	unsigned char c;
	int count = 0;

	while (true) {
		c = fgetc(in);
		if (feof(in)) break;

		++p[c];
		++count;
	}
	fseek(in, 0, SEEK_SET);

	return p;
}

int* read_occurrences(FILE *in) {
	int *p = new int[BYTE];

	for (int i = 0; i < BYTE; ++i) {
		fscanf(in, "%d", &p[i]);
	}

	return p;
}

void save_int_array(const int *a, int n) {
	FILE *out = fopen(TEMP_FILE_NAME, "w");

	for (int i = 0; i < n; ++i) {
		fprintf(out, "%d\n", a[i]);
	}

	fclose(out);
}

char** create_cipher() {
	char **cipher = (char**)malloc(sizeof(char*) * BYTE);

	for (int i = 0; i < BYTE; ++i) {
		cipher[i] = (char*)malloc(sizeof(char) * (BYTE + 1));
	}

	return cipher;
}

char** get_cipher(int *occurrencies) {
	huffman_list *list = NULL;
	huffman_list *new_obj;
	char **cipher = create_cipher();
	
	char **iterator = (char**)malloc(sizeof(char*) * BYTE);
	for (int i = 0; i < BYTE; ++i) { iterator[i] = cipher[i]; }

	for (int i = 0; i < BYTE; ++i) {
		if (occurrencies[i] > 0) {
			new_obj = create_list();
			new_obj->val = occurrencies[i];
			new_obj->symbols[0] = (char)i;
			new_obj->len = 1;

			add_list(&list, new_obj);
		}
	}

	if (list == NULL) { return cipher; }
	else if (list->next == NULL) {
		*(iterator[list->symbols[0]]++) = '1';
	}

	while (list->next != NULL) {
		new_obj = create_list();
		new_obj->val = list->val + list->next->val;
		combine_symbols(new_obj, list, list->next);

		for (int i = 0; i < list->len; ++i) {
			*(iterator[list->symbols[i]]++) = '0';
		}
		pop_list(&list);

		for (int i = 0; i < list->len; ++i) {
			*(iterator[list->symbols[i]]++) = '1';
		}
		pop_list(&list);

		add_list(&list, new_obj);
	}

	for (int i = 0; i < BYTE; ++i) {
		*iterator[i] = '\0';
		strrev(cipher[i]);
	}
	free(iterator);
	clear_list(&list);
	return cipher;
}

void free_cipher(char **cipher) {
	for (int i = 0; i < BYTE; ++i) { free(cipher[i]); }
	free(cipher);
}
