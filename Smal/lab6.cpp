
#include <stdio.h>
#include <stdlib.h>

struct node {
	int val;
	struct node *left, *right;
	int height;
};

int height(node* p)
{
	if (p)
		return p -> height;
	else 
		return 0;
}

void fixheight(node* p) {
	if (height(p->left) > height(p->right))
		p->height = height(p->left) + 1;
	else
		p->height = height(p->right) + 1;
}

int balance_factor(node* p) {
	if (p){
		if (p->left && p->right)
			return height(p->right) - height(p->left);
		if (p->left)
			return -height(p->left);
		if (p->right)
			return height(p->right);
	}
	return 0;
}

node* rotate_right(node* p) {
	if (p) {
		node* tmp = p->left;
		if (tmp) {
			p->left = tmp->right;
			tmp->right = p;
			fixheight(p);
			fixheight(tmp);
		}
		return tmp;
	}
	return NULL;
}

node* rotate_left(node* p) {
	if (p){
		node* tmp = p->right;
		if (tmp){
			p->right = tmp->left;
			tmp->left = p;
			fixheight(p);
			fixheight(tmp);
		}
		return tmp;
	}
	return NULL;
}

node* balance(node* p) {
	fixheight(p);
	if (balance_factor(p) == 2) {

		if (balance_factor(p->right) < 0) {								//Разобраться фактором
			p->right = rotate_right(p->right);
		}	
		return rotate_left(p);
	}

	if (balance_factor(p) == -2) {

		if (balance_factor(p->left) > 0) {
			p->left = rotate_left(p->left);
		}	
		return rotate_right(p);
	}
	return p;												//Если и так сбалансировано		
}

node* construct_node(node*p, int v) {
	struct node* new_node;
	new_node = (struct node*)malloc(sizeof(*new_node));
	new_node->height = 1;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->val = v;
	p = new_node;
	return balance(p);
}

node* insert_node(node* p, int v) {
	
	if (!p) {		
		return construct_node(p, v);
	}
	else {
		if (v < p->val){
			p->left = insert_node(p->left, v);
		}	
		else {
			p->right = insert_node(p->right,  v);
		}	
	}
	return balance(p);

}

void del_tree(struct node* p) {
	if (p) {
		if (p->left)
			del_tree(p->left);
		if (p->right)
			del_tree(p->right);
		free(p);
	}
}

int main()
{
	FILE *oFile;
	FILE *iFile;
	iFile = fopen("in.txt", "r");
	oFile = fopen("out.txt", "w");
	int n;
	fscanf(iFile, "%d", &n);
	if (n == 0)
		fprintf(oFile, "0");
	struct node* tree = NULL;
	int v;
	for (int i = 0; i < n; i++) {
		fscanf(iFile, "%d", &v);
		tree = insert_node(tree, v);
	}
	if (tree)
		fprintf(oFile, "%d", height(tree));
	del_tree(tree);
	fclose(iFile);
	fclose(oFile);
    return 0;
}

