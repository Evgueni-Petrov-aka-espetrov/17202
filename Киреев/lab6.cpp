#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
typedef struct node {
	int key;
	int height;
	struct node* left;
	struct node* right;
}node;

node* nodeConstructor(int key) {
	node* p = (node*)malloc(sizeof(node));
	p->height = 1;
	p->key = key;
	(*p).left = NULL;
	(*p).right = NULL;
	return p;
}

void nodeDestructor(node** p) {
	if ((*p)->left) {
		nodeDestructor(&((*p)->left));
	}
	if ((*p)->right) {
		nodeDestructor(&((*p)->right));
	}
	free(*p);
}

int height(node* p) {
	if (p != NULL) {
		return p->height;
	}
	else {
		return 0;
	}
}
int balanceFactor(node* p) {
	return height(p->right) - height(p->left);
}
void fixHeight(node* p) {
	int leftHeight = height(p->left);
	int rightHeight = height(p->right);
	if (leftHeight > rightHeight) {
		p->height = leftHeight + 1;
	}
	else {
		p->height = rightHeight + 1;
	}
}
node* leftRotation(node* p) {
	node* rightNode = p->right;
	p->right = rightNode->left;
	rightNode->left = p;
	fixHeight(p);
	fixHeight(rightNode);
	return rightNode;
}
node* rightRotation(node* p) {
	node* leftNode = p->left;
	p->left = leftNode->right;
	leftNode->right = p;
	fixHeight(p);
	fixHeight(leftNode);
	return leftNode;
}
node* balanceNode(node* p) {
	fixHeight(p);
	int bFactor = balanceFactor(p);
	if (bFactor == 2) {
		if (balanceFactor(p->right) < 0) {
			p->right = rightRotation(p->right);
		}
		return leftRotation(p);
	}
	else {
		if (bFactor == -2) {
			if (balanceFactor(p->left) > 0) {
				p->left = leftRotation(p->left);
			}
			return rightRotation(p);
		}
	}
	return p;
}
node* insertNode(node* p, int key) {
	if (p == NULL) {
		return nodeConstructor(key);
	}
	if (key < p->key) {
		p->left = insertNode(p->left, key);
	}
	else {
		p->right = insertNode(p->right, key);
	}
	return balanceNode(p);
}
node* readNode(FILE* input) {
	int count;
	fscanf(input, "%d", &count);
	if (count == 0) return NULL;
	node* p = NULL;
	for (int i = 0; i < count; ++i) {
		int tmp;
		fscanf(input, "%d", &tmp);
		p = insertNode(p, tmp);
	}
	return p;
}
int main() {
	FILE* input = fopen("in.txt", "r");

	node* p = readNode(input);
	fclose(input);

	if (p == NULL) {
		printf("%d", 0);
		return 0;
	}
	printf("%d", height(p));
	nodeDestructor(&p);
	return 0;
}
