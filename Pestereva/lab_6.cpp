#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int value;
	unsigned char h;
	node* left;
	node* right;
} node;

unsigned char height (node* N){
	if (N != NULL)
		return N->h;
	else
		return 0;
}

void calc_height(node *N) {
	if (height(N->left) > height(N->right))
		N->h = height(N->left) + 1;
	else
		N->h = height(N->right) + 1;
}

node* right_rotate(node* N) {
	node* K = N->left;
	N->left = K->right;
	K->right = N;
	calc_height(N);
	calc_height(K);
	return K;
}

node* left_rotate(node* N) {
	node* K = N->right;
	N->right = K->left;
	K->left = N;
	calc_height(N);
	calc_height(K);
	return K;
}

node* balance(node* N) {
	int dif = height(N->right) - height(N->left);
	if (dif == 2) {
		if (height(N->right->left) > height(N->right->right)) 
			N->right = right_rotate(N->right);
		return left_rotate(N);
	}
	if (dif == -2) {
		if (height(N->left->right) > height(N->left->left))
			N->left = left_rotate(N->left);
		return right_rotate(N);
	}
	return N;
}

node* create_node(int val) {
	node* N = new node();
	N->value = val;
	N->h = 1;
	N->left = NULL;
	N->right = NULL;
	return N;
}

node* insert(node* N, int val) {
	if (N == NULL)
		return create_node(val);
	if (val < N->value)
		N->left = insert(N->left, val);
	else
		N->right = insert(N->right, val);
	calc_height(N);
	return balance(N);
}

node* free(node* N) {
	if (N != NULL) {
		N->left = free(N->left);
		N->right = free(N->right);
		delete N;
	}
	return NULL;
}

int main() {
	int N;
	scanf_s("%d", &N);
	node* root=NULL;
	int val;
	for (int i = 0; i < N; i++) {
		scanf_s("%d", &val);
		root = insert(root, val);
	}
	printf("%d", height(root));
	root = free(root);
}