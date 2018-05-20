#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

typedef struct SS {
	char stack[MAX];
	int top;
} symstack;

typedef struct NS {
	int stack[MAX];
	int top;
} numstack;

char top_c(const symstack*c) {
	if ((c->top) > 0){
		return c->stack[c->top - 1];
	}
	return -1;
}

int top_int(const numstack*c) {
	if ((c->top) > 0){
		return c->stack[c->top - 1];
	}
	return -1;
}

void push(symstack*c, char el) {
	if (c->top < MAX){
		c->stack[c->top] = el;
		c->top++;
	}
}

void push(numstack*c, int el) {
	if (c->top < MAX){
		c->stack[c->top] = el;
		c->top++;
	}
}

char pop(symstack*c) {
	if ((c->top) > 0){
		c->top--;
		return c->stack[c->top];
	}
	return -1;
}

int pop(numstack*c) {
	if ((c->top) > 0){
		c->top--;
		return c->stack[c->top];
	}
	return -1;
}

int number(char a) {
	char * alph = "0123456789";
	for (int i = 0; i < 11; i++) {
		if (alph[i] == a) {
			return i;
		}
	}
	return -1;
}

int sign(char a) {
	char * alph = "()*/+-";
	for (int i = 0; i < 6; i++) {
		if (alph[i] == a) {
			return 1;
		}
	}
	return 0;
}

char priority(char sgn) {
	switch (sgn) {
	case '(':
	case ')':
		return 1;
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	}
	return -1;
}

int calc_symb(numstack* val, char symb) {
	if (val->top < 2) {
		printf("syntax error");
		return 0;
	}
	if (symb == '/'&&top_int(val) == 0) {
		printf("division by zero");
		return 0;
	}
	int a = pop(val);
	int b = pop(val);
	switch (symb) {
	case '+':
		push(val, a + b);
		break;
	case '-':
		push(val, b - a);
		break;
	case '*':
		push(val, a*b);
		break;
	case '/':
		push(val, b / a);
		break;
	}
	return 1;
}

int calculator(FILE *in, int *res) {
	int count = 0;
	numstack num;
	num.top = 0;
	symstack sym;
	sym.top = 0;
	while (1) {
		int a = fgetc(in);
		if ((char)a == '\n' || a == EOF) {
			break;
		}
		int val = number(a);
		if (val >= 0) {
			if (count == 1) {
				push(&num, pop(&num) * 10 + val);
			}
			else {
				push(&num, val);
			}
			count = 1;
		}
		else if (sign(a) == 1) {
			if (count == 1 && (char)a == '(') {
				return 0;
			}
			if (count == 2 && (char)a == ')' && top_c(&sym) == '(') {
				return 0;
			}
			count = 0;
			if ((char)a == '(') {
				count = 2;
				push(&sym, a);
			}
			else if ((char)a == ')') {
				while (sym.top > 0 && top_c(&sym) != '(') {
					if (calc_symb(&num, pop(&sym)) == 0) {
						return 0;
					}
				}
				if (sym.top > 0 && top_c(&sym) == '(') {
					pop(&sym);
				}
				else {
					return 0;
				}
			}
			else {
				while (sym.top > 0 && priority(top_c(&sym)) >= priority(a)){
					if (calc_symb(&num, pop(&sym)) == 0) {
						return 0;
					}
				}
				push(&sym, a);
			}
		}
		else {
			return 0;
		}
	}
	while (sym.top > 0) {
		if (calc_symb(&num, pop(&sym)) == 0) {
			return 0;
		}
	}
	if (num.top == 1)
		*res = pop(&num);
	else {
		return 0;
	}
	return 1;
}

int main(){
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	int res;
	if (calculator(in, &res) == 1)
		fprintf(out, "%d", res);
	else
		fprintf(out, "syntax error");

	fclose(in);
	fclose(out);

	return 0;
}
