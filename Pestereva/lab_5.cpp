#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <windows.h>

const int table_size = 256;

typedef struct node
{
	int rang;
	unsigned char ch;
	node* left;
	node* right;
	node* next;
} node;

typedef struct scode {
	unsigned short code;
	unsigned char len;
} scode;

node* create_node(int rang, unsigned char ch) {
	node* N = (node*)malloc(sizeof(node));
	N->rang = rang;
	N->ch = ch;
	N->left = NULL;
	N->right = NULL;
	N->next = NULL;
	return N;
}

node* insert2list(node* head, node* newn) {
	if (head == NULL || head->rang >= newn->rang) {
		newn->next = head;
		return newn;
	}
	node* N = head;
	while (N->next != NULL && N->next->rang < newn->rang) {
		N = N->next;
	}
	newn->next = N->next;
	N->next = newn;
	return head;
}

node* create_list(int *table) {
	node* head = NULL;
	for (int i = 0; i < table_size; i++) {
		if (table[i] != 0) {
			node* N = create_node(table[i], i);
			head = insert2list(head, N);
		}
	}
	return head;
}

node* create_node(node *N1, node *N2) {
	node* N = (node*)malloc(sizeof(node));
	N->rang = N1->rang + N2->rang;
	N->ch = 254;
	N->left = N1;
	N->right = N2;
	N->next = NULL;
	return N;
}

node* create_tree(node* head) {
	if (head == NULL || head->next == NULL) {
		return head;
	}
	node* newn = create_node(head, head->next);
	head = head->next->next;
	head = insert2list(head, newn);
	create_tree(head);
}

void create_codes(scode* codes, const node* node, unsigned short code, unsigned char len) {
	if (node->left == NULL) {
		codes[node->ch].code = code;
		codes[node->ch].len = len == 0 ? len + 1 : len;
	}
	else {
		create_codes(codes, node->left, (code << 1) + 1, len + 1);
		create_codes(codes, node->right, code << 1, len + 1);
	}
}
void zero_codes(scode* codes) {
	for (int i = 0; i < table_size; i++) {
		codes[i].len = 0;
	}
}

void printco(const scode* codes, const int* table) {
	for (int i = 0; i < table_size; i++) {
		if (codes[i].len != 0)
			printf("%d %d %d %d\n", i, codes[i].code, codes[i].len, table[i]);
	}
}

void zero_buf(int table[]) {
	memset(table, 0, 256 * sizeof(int));
}

void count_ch(FILE* in, FILE* tmp, int table[]) {
	unsigned char byte;
	while (fread(&byte, sizeof(unsigned char), 1, in)) {
		table[byte]++;
		fwrite(&byte, sizeof(unsigned char), 1, tmp);
	}
}

void printarr(const int table[]) {
	for (int i = 0; i < 256; i++) {
		printf("%d ", table[i]);
	}
}

void wrtree(FILE* out, const node* node) {
	fwrite(&(node->ch), sizeof(unsigned char), 1, out);
	unsigned char has_sp;
	if (node->left == NULL) {
		has_sp = 0;
		fwrite(&has_sp, sizeof(unsigned char), 1, out);
	}
	else {
		has_sp = 1;
		fwrite(&has_sp, sizeof(unsigned char), 1, out);
		wrtree(out, node->left);
		wrtree(out, node->right);
	}
}

node* retree(FILE* in) {
	int nrang;
	unsigned char nch;
	unsigned char has_sp;
	fread(&nch, sizeof(unsigned char), 1, in);
	fread(&has_sp, sizeof(unsigned char), 1, in);
	node* n;
	n = create_node(0, nch);
	if (has_sp == 1) {
		n->left = retree(in);
		n->right = retree(in);
	}
	return n;
}

void encode(FILE* tmp, const scode codes[], FILE* out) {
	unsigned int ebuf = 0;
	unsigned char byte;
	unsigned char freeb = 32;
	while (fread(&byte, sizeof(unsigned char), 1, tmp) > 0) {
		if (codes[byte].len <= freeb) {
			ebuf |= (codes[byte].code << (freeb - codes[byte].len));
			freeb -= codes[byte].len;
		}
		else {
			ebuf |= (codes[byte].code >> (codes[byte].len - freeb));
			fwrite(&ebuf, sizeof(unsigned int), 1, out);
			freeb = 32 + freeb - codes[byte].len;
			ebuf = codes[byte].code << freeb;
		}
	}
	if (freeb < 32) {
		fwrite(&ebuf, sizeof(unsigned int), 1, out);
	}
}

void decode(FILE* in, FILE* out, const node* root, int count) {
	unsigned int coded = 0;
	const node* node = root;
	while (fread(&coded, sizeof(unsigned int), 1, in) > 0) {
		for (int i = 31; i >= 0; i--) {
			if (count == 0) break;
			if (node->left == NULL) {
				fwrite(&(node->ch), sizeof(unsigned char), 1, out);
				count--;
				node = root;
			}
			else {
				if (((coded >> i) & 1) == 1) {
					node = node->left;
				}
				else
					node = node->right;
				if (node->left == NULL) {
					fwrite(&(node->ch), sizeof(unsigned char), 1, out);
					count--;
					node = root;
				}
			}
		}
	}
}

void zcode(FILE* in, FILE* out) {
	unsigned char buf[1024];
	int rcount = 0;
	do {
		rcount = fread(buf, sizeof(unsigned char), 1024, in);
		fwrite(buf, sizeof(unsigned char), rcount, out);
	} while (rcount > 0);
}

int estimated_size(const scode* scodes, const int* table) {
	int size = 0;
	for (int i = 0; i < 256; i++) {
		size += table[i] * scodes[i].len;
	}
	return (size + 7) / 8;
}

int main() {
	FILE* in = stdin;
	FILE* out = stdout;
	setvbuf(in, NULL, _IOFBF, 1024);
	setvbuf(out, NULL, _IOFBF, 1024);

	unsigned char mode[3];
	_setmode(_fileno(in), O_BINARY);
	_setmode(_fileno(out), O_BINARY);
	fread(mode, sizeof(unsigned char), 3, in);



	if (mode[0] == 'c') {
		int table[table_size];
		zero_buf(table);
		FILE* tmp;
		errno_t err = fopen_s(&tmp, "buf.tmp", "wb");
		setvbuf(tmp, NULL, _IOFBF, 1024);
		count_ch(in, tmp, table);
		err = fclose(tmp);
		node* head = create_list(table);
		node* root = create_tree(head);
		if (root != NULL) {
			scode codes[table_size];
			zero_codes(codes);
			create_codes(codes, root, 0, 0);
			//printco(codes, table);
			err = fopen_s(&tmp, "buf.tmp", "rb");
			setvbuf(tmp, NULL, _IOFBF, 1024);
			if (root->rang <= estimated_size(codes, table)) {
				unsigned char zcoding = 1;
				fwrite(&(zcoding), sizeof(unsigned char), 1, out);
				zcode(tmp, out);
			}
			else {
				unsigned char zcoding = 0;
				fwrite(&(zcoding), sizeof(unsigned char), 1, out);
				if (root != NULL) fwrite(&(root->rang), sizeof(int), 1, out);
				wrtree(out, root);
				encode(tmp, codes, out);
			}
			fclose(tmp);
		}
	}
	else if (mode[0] == 'd') {
		int count;
		unsigned char zencoding;
		fread(&zencoding, sizeof(unsigned char), 1, in);
		if (zencoding == 1) {
			zcode(in, out);
		}
		else {
			fread(&count, sizeof(int), 1, in);
			node* newroot = retree(in);
			decode(in, out, newroot, count);
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}
