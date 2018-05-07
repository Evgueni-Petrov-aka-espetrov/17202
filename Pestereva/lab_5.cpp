#include <stdio.h>
#include <stdlib.h>
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

node* create_node(int rang, unsigned char ch){
	node* N = (node*)malloc(sizeof(node));
	N->rang = rang;
	N->ch = ch;
	N->left = NULL;
	N->right = NULL;
	N->next = NULL;
	return N;
}

node* insert2list(node* head, node* newn) {
	if (head == NULL || head->rang >=newn->rang) {
		newn->next = head;
		return newn;
	}
	node* N = head;
	while (N->next!=NULL && N->next->rang < newn->rang) {
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
	N->ch = NULL;
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
		codes[node->ch].len = len;
	}
	else {
		create_codes(codes, node->left, (code << 1) + 1, len + 1);
		create_codes(codes, node->right, code << 1 , len + 1);
	}
}
void zero_codes(scode* codes) {
	for (int i = 0; i < table_size; i++) {
		codes[i].len = 0;
	}
}

void printco(scode* codes, int* table) {
	for (int i = 0; i < table_size; i++) {
		if (codes[i].len!=0)
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

void printarr(int table[]) {
	for (int i = 0; i < 256; i++) {
		printf("%d ", table[i]);
	}
}

void wrtree(FILE* out, node* node) {
	//fwrite(&(node->rang), sizeof(int), 1, out);
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
	//fread(&nrang, sizeof(int), 1, in);
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

void encode (FILE* tmp, const scode codes[], FILE* out) {
	unsigned short ebuf = 0;
	unsigned char byte;
	unsigned char freeb = 16;
	while (fread(&byte, sizeof(unsigned char), 1, tmp) > 0){
		if (codes[byte].len <= freeb) {
			ebuf |= (codes[byte].code << (freeb - codes[byte].len));
			freeb -= codes[byte].len;
		}
		else {
			ebuf |= (codes[byte].code >> (codes[byte].len - freeb));
			fwrite (&ebuf, sizeof(unsigned short), 1, out);
			freeb = 16 + freeb - codes[byte].len;
			ebuf = codes[byte].code << freeb;
		}
	}
	if (freeb < 16) {
		//if (ebuf == 0xffff) ebuf = 0;
		fwrite(&ebuf, sizeof(unsigned short), 1, out);
	}
}

void decode(FILE* in, FILE* out, node* root) {
	unsigned short byte = 0;
	node* node = NULL;
	unsigned char prev = 0;
	while (fread(&byte, sizeof(unsigned short), 1, in) > 0) {
		for (int i = 15; i >= 0; i--) {
			if (node == NULL) {
				if (((byte >> i) & 1) == 1) {
					node = root;
				}
				else
					break;
			}
			else {
				if (((byte >> i) & 1) == 1) {
					node = node->left;
				}
				else
					node = node->right;
			}
			if (node->left == NULL) {
				/*if (node->ch == 10 && prev != 13) {
					unsigned char r = 13;
					fwrite(&r, sizeof(unsigned char), 1, out);
				}*/
				fwrite(&(node->ch), sizeof(unsigned char), 1, out);
				prev = node->ch;
				node = NULL;
			}
		}
	}
}

int main() {
	FILE* in;
	FILE* out;
	//CopyFile("___in.txt", "____in.txt", 0);
	//CopyFile("__in.txt", "___in.txt", 0);
	CopyFile("_in.txt", "__in.txt", 0);
	CopyFile("in.txt", "_in.txt", 0);
	//freopen(NULL, "rb", stdin);
	in = stdin;
	unsigned char mode[3];
	//puts(mode);
	_setmode(_fileno(in), O_BINARY);
	fread(mode, sizeof(unsigned char), 3, in);
	//out = freopen(NULL, "wb", stdout);
	out = stdout;
	_setmode(_fileno(out), O_BINARY);

	if (mode[0] == 'c') {
		int table[table_size];
		zero_buf(table);
		FILE* tmp;
		errno_t err = fopen_s(&tmp, "buf.tmp", "wb");
		count_ch(in, tmp, table);
		fclose(tmp);
		node* head = create_list(table);
		node* root = create_tree(head);
		if (root != NULL) {
			scode codes[table_size];
			zero_codes(codes);
			create_codes(codes, root, 1, 1);
			wrtree(out, root);
			err = fopen_s(&tmp, "buf.tmp", "rb");
			encode(tmp, codes, out);
			fclose(tmp);
			fclose(out);
		}
	}
	else if (mode[0] == 'd') {
		node* newroot = retree(in);
		decode(in, out, newroot);
	}
	return 0;
}