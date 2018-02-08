#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

struct tree {
	int key;
	unsigned char hight;
	struct tree *l, *r;
};

unsigned char hight(struct tree* t) {
	return t ? t->hight : 0;
}

void setHight(struct tree* t) {
	unsigned char highLeft = hight(t->l);
	unsigned char highRight = hight(t->r);
	if (highLeft > highRight) {
		t->hight = highLeft + 1;
	}
	else {
		t->hight = highRight + 1;
	}
}

int bFactor(struct tree* t) {
	return hight(t->r) - hight(t->l);
}

struct tree* lrot(tree* t) {
	struct tree *balanced = t->r;
	t->r = balanced->l;
	balanced->l = t;
	setHight(t);
	setHight(balanced);
	return balanced;
}

struct tree* rrot(struct tree* t) {
	struct tree *balanced = t->l;
	t->l = balanced->r;
	balanced->r = t;
	setHight(t);
	setHight(balanced);
	return balanced;
}

struct tree* balance(struct tree* t) {
	setHight(t);
	int b = bFactor(t);
	if (b == 2) {
		if (bFactor(t->r) < 0) {
			t->r = rrot(t->r);
		}
		t = lrot(t);
	}

	if (b == -2){
		if (bFactor(t->l) > 0) {
			t->l = lrot(t->l);
		}
		t = rrot(t);
	}
	return t;
}

struct tree* constructorTree(struct tree* t, int k) {
	t = (struct tree*)malloc(sizeof(struct tree));
	t->key = k;
	t->l = NULL;
	t->r = NULL;
	t->hight = 1;
	return balance(t);
}

struct tree* insert(struct tree* t, int k) {
	if (!t) {
		return constructorTree(t, k);
	}
	else {
		if (k < t->key) {
			t->l = insert(t->l, k);
		}
		else {
			t->r = insert(t->r, k);
		}
	}
	return balance(t);
}

struct tree* read(FILE* stream, int N) {	
	struct tree* t = NULL;
	for (int i = 0; i < N; ++i) {
		int k;
		fscanf(stream, "%d", &k);
		t = insert(t, k);
	}
	fclose(stream);
	return t;
}

void balanceTree() {
	FILE* stream = fopen("in.txt", "r");
	FILE* fout = fopen("out.txt", "w");
	int N;
	fscanf(stream, "%d", &N);
	if (N == 0) { 
		fprintf(fout, "0"); 
		return; 
	}
	struct tree* t = read(stream, N);
	fprintf(fout, "%d", t->hight);
	fclose(fout);
}

int main() {
	balanceTree();
	return 0;
}
