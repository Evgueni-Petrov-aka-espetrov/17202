#include <stdio.h>
#include <stdlib.h>
#define Byte unsigned char

typedef struct CodSym {
	Byte length;
	long long code;
} CodSym;

typedef struct Node {
	struct Node* left;
	struct Node* right;
	long long code;
	Byte sym;
} Node;

typedef struct TNode {
	struct TNode* next;
	Node* node;
} TNode;

typedef struct Ch {
	Byte sym;
	int quan;
} Ch;

void check_malloc(void* ptr) {
	if (ptr == NULL) {
		printf("Not enough memory");
		exit(0);
	}
}

void code_sym(CodSym s, FILE* file) {
	static Byte ch = 0;
	static int k = 7;

	for (int i = s.length - 1; i >= 0; i--) {
		ch |= ((s.code >> i) & 1) << k;
		k--;
		if (k == -1) {
			k = 7;
			fputc(ch, file);
			ch = 0;
		}
	}
	return;
}

void code_text(char* name_in, CodSym* cods, Ch* chars, int v) {
	FILE* output = fopen("out.txt", "wb");
	if (output == NULL) {
		printf("File open error\n");
		exit(2);
	}
	FILE* input = fopen(name_in, "rb");
	if (input == NULL) {
		printf("File open error\n");
		exit(1);
	}
	fgetc(input);
	fgetc(input);
	fgetc(input);
	

	fprintf(output, "%d", v);
	int count_sym = 0;
	for (int i = 0; i < 256; ++i) {
		if (chars[i].quan != 0) {
			count_sym++;
		}
	}
	if (count_sym == 256) {
		fputc('=', output);
	}
	else {
		fputc(' ', output);
		for (int i = 0; i < 256; ++i) {
			if (chars[i].quan != 0) {
				fputc(chars[i].sym, output);
				fprintf(output, "%d", chars[i].quan);
				fputc(' ', output);
			}
		}
	}

	int ch = fgetc(input);

	while (ch != EOF) {
		code_sym(cods[ch], output);
		CodSym c = cods[ch];
		ch = fgetc(input);
	}
	CodSym help = { 1 , 0 };

	for (int i = 0; i < 7; ++i) {
		code_sym(help, output);
	}
	fclose(output);
	fclose(input);
	return;
}

int compare(const void* first, const void* second) {
	Ch* tmp_first = (Ch*)first;
	Ch* tmp_second = (Ch*)second;

	if (tmp_first->quan == tmp_second->quan) {
		return 0;
	}
	else return (tmp_first->quan > tmp_second->quan) ? 1 : -1;

}

Ch* get_quan(FILE* input, int* v) {
	*v = 0;
	Ch* quan = (Ch*)calloc(256, sizeof(Ch));
	check_malloc(quan);
	for (int i = 0; i < 256; i++) {
		quan[i].sym = i;
	}

	int tmp = fgetc(input);// int tmp = getc();

	if (tmp == EOF) {
		free(quan);
		exit(0);
	}
	while (tmp != EOF) {
		quan[(unsigned char)tmp].quan++;
		tmp = fgetc(input);
		++(*v);

	}
	Ch x = quan[32];
	fclose(input);
	return quan;
}


TNode* push_tree_list(TNode* root, Ch ch, Node* left, Node* right) {
	TNode* help = (TNode*)malloc(sizeof(TNode));
	help->node = (Node*)malloc(sizeof(Node));

	check_malloc(help);
	check_malloc(help->node);

	help->node->left = left;//making copy
	help->node->right = right;
	help->node->sym = ch.sym;
	help->node->code = ch.quan;

	if (root == NULL) {
		root = help;
		help->next = NULL;
		return help;
	}


	TNode* local_root = root->next;//finding place to insert
	TNode* previous_local = root;
	while ((local_root != NULL) && (local_root->node->code < ch.quan))
	{
		previous_local = local_root;
		local_root = local_root->next;
	}

	if (previous_local != root) {

		help->next = previous_local->next;
		previous_local->next = help;
	}
	else {
		help->next = root;
		root = help;
	}

	//inserting

	return (root == NULL) ? help : root;
}
void free_tnode(TNode* tnode) {
	free(tnode->node);
	free(tnode);
}

TNode* merge_push(TNode* root, TNode* c1, TNode* c2) {
	Ch tmp;//creating unit
	tmp.quan = c1->node->code + c2->node->code;
	tmp.sym = 0;
	int s = c1->node->code;
	TNode* result = push_tree_list(root, tmp, c1->node, c2->node);
	result = root->next->next;
	free(c2);
	free(c1);
	return result;

}

void print_tree(Node* tree) {
	if (tree == NULL) {
		return;
	}
	if ((tree->left == NULL) && (tree->right == NULL)) {
		fprintf(stderr, "%c %d\n", (char)tree->sym, tree->code);
	}
	print_tree(tree->left);
	print_tree(tree->right);
	return;
}

void free_tree(Node* tree) {
	if (tree == NULL) {
		return;
	}
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);

	return;
}

void code_tree(Node* tree, CodSym* arr, long long cur_code, int len) {
	if (tree == NULL) {
		return;
	}
	if (len >= 22) {
		int x = 0;
	}
	code_tree(tree->right, arr, (cur_code << 1) | 1, len + 1);
	code_tree(tree->left, arr, (cur_code << 1) & (~1), len + 1);
	if ((tree->left == NULL) && (tree->right == NULL)) {
		arr[tree->sym].code = cur_code;
		arr[tree->sym].length = len;
	}

	return;
}

Node* get_tree(Ch* arr) {
	TNode* tree_list = NULL;
	for (int i = 0; i < 256; ++i) {
		if (arr[i].quan != 0) {//getting list of signified structures
			tree_list = push_tree_list(tree_list, arr[i], NULL, NULL);
			int x = 0;
		}
		if (i > 250) {
			int y = 0;
		}
	}
	int x = 0;
	if (tree_list->next == NULL) {
		Node* help = malloc(sizeof(Node));
		check_malloc(help);
		help->code = tree_list->node->code;
		help->sym = 0;
		help->right = tree_list->node;
		help->left = NULL;
		return help;
	}
	while (tree_list->next != NULL) {//until we have one full tree
		tree_list = merge_push(tree_list, tree_list, tree_list->next);
	}
	return tree_list->node;
}

int decode_sym(FILE* name, int c, Node* tree, int v) {
	static int num_decoded = 0;
	static times = 1;
	static Node* tmp_node = NULL;
	if (tree == 0) {
		exit(0);
	}
	if (times == 1) {
		tmp_node = tree;
	}


	if (c == 0) {
		tmp_node = tmp_node->left;
	}
	else {
		tmp_node = tmp_node->right;
	}

	if (tmp_node->left == NULL && tmp_node->right == NULL) {

		fputc((char)(tmp_node->sym), name);

		++num_decoded;
		tmp_node = tree;
		if (num_decoded == v) {
			return 1;
		}
	}
	times++;
	return 0;
}

void decode_text(FILE* input, char* name_out, Node* tree, int v) {

	FILE* output = fopen(name_out, "wb");

	int tmp_coded = fgetc(input);
	while (tmp_coded != EOF) {
		for (int i = 7; i >= 0; --i) {//uncoding by bytes
			if (decode_sym(output, ((Byte)tmp_coded >> i) & 1, tree, v)) {
				break;
			}
		}
		tmp_coded = fgetc(input);
	}

	fclose(output);
}

Ch* get_chars(FILE* input, int* v) {
	Ch* quan = (Ch*)calloc(256, sizeof(Ch));
	check_malloc(quan);
	for (int i = 0; i < 256; i++) {
		quan[i].sym = i;
	}


	fscanf(input, "%d", v);
	int tmp_v = 0;

	int tmp = fgetc(input);
	if (tmp == EOF) {
		free(quan);
		exit(0);
	}
	if (tmp == '=') {
		for (int i = 0; i < 256; i++) {
			quan[i].quan = 256;
		}
	}
	else {
		while ((tmp != EOF) && (*v != tmp_v)) {
			fscanf(input, "%c", &tmp);
			
			fscanf(input, "%d", &quan[(Byte)tmp].quan);
			Ch x = quan[tmp];
			fgetc(input);
			tmp_v += quan[(Byte)tmp].quan;
		}
	}
	

	return quan;
}


int main() {
	FILE* input = fopen("in.txt", "rb");
	if (input == NULL) {
		printf("File open error\n");
		exit(1);
	}
	
	char action;
	fscanf(input, "%c", &action);
	char dev = fgetc(input);
	dev = fgetc(input);
	if (action == 'c') {
		//_sleep(1137);
		int v;
		Ch* chars = get_quan(input, &v);

		qsort(chars, 256, sizeof(Ch), compare);

		Node* tree = get_tree(chars);


		CodSym* cods = calloc(256, sizeof(CodSym));
		code_tree(tree, cods, 0, 0);
		for (int i = 0; i < 255; i++) {
			CodSym x = cods[i];
			int y = 0;
		}
		code_text("in.txt", cods, chars, v);
		
		free(chars);
		free_tree(tree);
		free(cods);


	}
	if (action == 'd') {
		int v;
		Ch* chars = get_chars(input, &v);
		
		Ch x = chars[32];
		x = chars[0];
		qsort(chars, 256, sizeof(Ch), compare);

		Node* tree = get_tree(chars);

		decode_text(input, "out.txt", tree, v);

	}
	fclose(input);
	return 0;
}

