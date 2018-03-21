#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bit_stream.h"
#include "binary_tree.h"
#include "huffman_list.h"

const int BYTE = 256;
const char* TEMP_FILE_NAME = "saved_ocur.txt";

void encode(FILE *in, FILE *out, char **cipher);
void decode(FILE *in, FILE *out, char **cipher, int count);

int get_total_occurrences(const int *occurrences);
int* get_occurrences(FILE *in);
void save_occurrences(const int *ocr);
int* read_occurrences();

char** create_cipher();
char** get_cipher(int *occurrencies);
void free_cipher(char **cipher);

int main() {
	FILE *in, *out;
	in = fopen("in.txt", "rb");
	out = fopen("out.txt", "wb");

	int *ocr;
	char **cipher;
	char mode[4];


	fgets(mode, 4, in);

	if (mode[0] == 'c') {
		ocr = get_occurrences(in);
		cipher = get_cipher(ocr);

		fgets(mode, 4, in);
		encode(in, out, cipher);
		save_occurrences(ocr);

		free(ocr);
		free_cipher(cipher);
	}
	else if (mode[0] == 'd') {
		ocr = read_occurrences();
		cipher = get_cipher(ocr);

		decode(in, out, cipher, get_total_occurrences(ocr));

		free(ocr);
		free_cipher(cipher);
	}

	fclose(in);
	fclose(out);
	return 0;
}

void encode(FILE *in, FILE *out, char **cipher) {
	bit_stream *stream = create_stream(out);
	unsigned char c;

	while (true) {
		c = fgetc(in);
		if (feof(in)) break;

		put_bits(stream, cipher[c]);
	}
	print_remainder(stream);

	free_stream(stream);
}

void decode(FILE *in, FILE *out, char **cipher, int count) {
	bit_stream *stream = create_stream(in);
	binary_tree *tree = build_tree(cipher, BYTE);
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
	free_stream(stream);
}

int get_total_occurrences(const int *occurrences) {
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

void save_occurrences(const int *ocr) {
	FILE *out = fopen(TEMP_FILE_NAME, "w");

	for (int i = 0; i < BYTE; ++i) {
		fprintf(out, "%d\n", ocr[i]);
	}

	fclose(out);
}

int* read_occurrences() {
	FILE *in = fopen(TEMP_FILE_NAME, "r");
	int *ocr = new int[BYTE];

	for (int i = 0; i < BYTE; ++i) {
		fscanf(in, "%d", &ocr[i]);
	}

	fclose(in);
	return ocr;
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
			new_obj = create_list(BYTE);
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
		new_obj = create_list(BYTE);
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
