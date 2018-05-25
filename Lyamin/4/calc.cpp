#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

enum {DIGIT, OPENED_BRACKET, OPERATION, CLOSED_BRACKET, ERROR};

struct op_stack {
	char op;
	struct op_stack* next;
};

struct val_stack {
	int value;
	struct val_stack *next;
};

typedef struct op_stack op_stack;
typedef struct val_stack val_stack;

op_stack* op_stack_ctor(char op) {
	op_stack* result = (op_stack*)malloc(sizeof(op_stack));
	result->op = op;
	result->next = NULL;
	return result;
}

val_stack* val_stack_ctor(int value) {
	val_stack* result = (val_stack*)malloc(sizeof(val_stack));
	result->value = value;
	result->next = NULL;
	return result;
}

void push_op_to_stack(op_stack** prev, char op) {
	op_stack* new_op = op_stack_ctor(op);
	if(!prev) { 
		prev = &new_op;
		return;
	}
	new_op->next = *prev;
	*prev = new_op;
	return;
}

void push_val_to_stack(val_stack** prev, int value) {
	val_stack* new_val = val_stack_ctor(value);
	if (!prev) {
		*prev = new_val;
		return;
	}
	new_val->next = *prev;
	*prev = new_val;
	return;
}

int get_val_from_stack(val_stack** values) {
	int result = (*values)->value;
	val_stack* tmp = (*values)->next;
	free(*values);
	*values = tmp;
	return result;
}

char get_op_from_stack(op_stack** operations) {
	char result = (*operations)->op;
	op_stack* tmp = (*operations)->next;
	free(*operations);
	*operations = tmp;
	return result;
}

int translation(char c) {
	if (c >= '0' && c <= '9') { return DIGIT; } //numbers is 0
	if (c == '(') { return OPENED_BRACKET; } // open bracket is 1
	if (c == '+' || c == '-' || c == '*' || c == '/') { return OPERATION; } // operations is 2
	if (c == ')') { return CLOSED_BRACKET; } // closed bracket 3
	int syntax_error = ERROR; // AHHHHH ERRRRRORRRRRRRRRrr 4
	return syntax_error;
}

int char_to_int(char c) {
	int alphabet[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int i = 0; i < 10; ++i) {
		if (alphabet[i] == c) {
			return i;
		}
	}
	fprintf(stderr, "error\n");
	return -1;
}

void change_brackets(char cur, int* brackets) {
	if (translation(cur) == OPENED_BRACKET) { ++(*brackets); }
	if (translation(cur) == CLOSED_BRACKET) { --(*brackets); }
	return;
}

int valid_brackets(int* brackets) {
	if ((*brackets) < 0) { return 0; }
	return 1;
}

int valid_first_symbol(char first, int* brackets) {
	if (translation(first) == OPERATION || translation(first) == CLOSED_BRACKET || translation(first) == ERROR) { return 0; } //operator or ')' or syntax error
	return 1;
}

int valid_between_symbol(char prev, char cur, int* brackets) {
	if (translation(cur) == ERROR) { return 0; } // syntax error
	if (translation(prev) == DIGIT && translation(cur) == OPENED_BRACKET) { return 0; } // number and closed bracket
	if (translation(prev) == OPERATION && translation(cur) == OPERATION) { return 0; } // two operators
	if (translation(prev) == OPENED_BRACKET && translation(cur) == CLOSED_BRACKET) { return 0; } // two brackets 
	if (translation(prev) == OPERATION && translation(cur) == CLOSED_BRACKET) { return 0; } // operator and bracket
	return 1;
}
int valid_last_symbol(char last, int* brackets) {
	if (translation(last) == 1 || translation(last) == 2 || translation(last) == 4 ) { return 0; } // '(' or operator or syntax error
	return 1;
}
int valid_expression(FILE* input) {
	int brackets = 0;
	char buff;
	char prev;
	int counter = 0;
	while (true) {
		buff = fgetc(input);
		if (buff == EOF) { break; }
		if (buff == '\n') { continue; }

		change_brackets(buff, &brackets);
		if (!valid_brackets(&brackets)) { return 0; }

		if (counter == 0) {
			if (!valid_first_symbol(buff, &brackets)) { return 0; }
			++counter;
		}
		else {
			if (!valid_between_symbol(prev, buff, &brackets)) { return 0; }
		}
		prev = buff;
	}
	
	if (counter == 0) { return 0; }

	if (!valid_last_symbol(prev, &brackets)) { return 0; }
	if (brackets != 0) { return 0; }

	fseek(input, 0, SEEK_SET);
	return 1;
}

int opert(char op, int val_l, int val_r, int* division_by_zero) {
	switch(op) {
		case '+': { return val_l + val_r; } break;
		case '-': { return val_l - val_r; } break;
		case '*': { return val_l * val_r; } break;
		case '/': { 
			if (val_r == 0) {
				*division_by_zero = 1;
				return 0;
			}
			else{ return val_l / val_r; }
		} break;
	}

	fprintf(stderr, "\nunknown op error\n");
	return 0;
}

void operations_from_stacks(op_stack** operations, val_stack** values, int* division_by_zero) {
	int val_r = get_val_from_stack(values);
	int val_l = get_val_from_stack(values);
	char op = get_op_from_stack(operations);
	int result = opert(op, val_l, val_r, division_by_zero);
	push_val_to_stack(values, result);
	return;
}

void calc(FILE* input, FILE* output) {
	op_stack* operations = NULL;
	val_stack* values = NULL;

	int division_by_zero = 0;
	while (true) {

		if (division_by_zero == 1) { fprintf(output, "division by zero"); return; }
		char buff = fgetc(input);
		if (buff == EOF || feof(input)) { break; }
		if (buff == '\n') { continue; }

		switch (translation(buff)) {
			case DIGIT: {
				fseek(input, -1, SEEK_CUR);
				int tmp;
				fscanf(input, "%d", &tmp);
				push_val_to_stack(&values, tmp);
			}	break;

			case OPENED_BRACKET: {
				push_op_to_stack(&operations, buff);
			}	break;

			case OPERATION: {
				if (buff == '+' || buff == '-') {
					while (operations && values) {
						if (operations->op == '*' || operations->op == '/')
							operations_from_stacks(&operations, &values, &division_by_zero);
						else { break; }
					}
					if (operations && values) {
						if (operations->op == '-') {
							operations_from_stacks(&operations, &values, &division_by_zero);
						}
					}

					push_op_to_stack(&operations, buff);
				}

				if (buff == '*' || buff == '/') {
					if (operations) {
						if (operations->op == '*' || operations->op == '/') {
							operations_from_stacks(&operations, &values, &division_by_zero);
						}
					}
					push_op_to_stack(&operations, buff);
				}
			}	break;

			case CLOSED_BRACKET: {
				while (operations->op != '(') {
					operations_from_stacks(&operations, &values, &division_by_zero);
				}
				get_op_from_stack(&operations);
			}	break;

			case ERROR: {
				fprintf(stderr, "something wrong with you");
				return;
			}	break;
		}
	}

	while (operations && values)
		operations_from_stacks(&operations, &values, &division_by_zero);

	if (division_by_zero) {
		fprintf(output, "division by zero");
		return;
	}
	else{
		if (values) {
			fprintf(output, "%d", values->value);
			return;
		}
	}
	return;
}

int main() {
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	if (!valid_expression(input)) { 
		fprintf(output, "syntax error"); 
		return 0; 
	}
	calc(input, output);
}
