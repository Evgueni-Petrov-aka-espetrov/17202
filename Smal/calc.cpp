#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

 
struct val_stack {
	int val;
	struct val_stack* next;
};

struct op_stack {
	char oper;
	struct op_stack* next;
};

struct op_stack* Construct_op(char c) {
	struct op_stack* new_op = (struct op_stack*)malloc(sizeof(struct op_stack));
	new_op->oper = c;
	new_op->next = NULL;
	return new_op;
}

struct val_stack* Construct_val(int val) {
	struct val_stack* new_val = (struct val_stack*)malloc(sizeof(struct val_stack));
	new_val->val = val;
	new_val->next = NULL;
	return new_val;
}

void push_oper(char op, struct op_stack** last) {
	struct op_stack* new_elem = Construct_op(op);
	new_elem->next = *last;
	*last = new_elem;
}

void push_val(int val, struct val_stack** last) {
	struct val_stack* new_elem = Construct_val(val);
	new_elem->next = *last;
	(*last) = new_elem;
}

int pop_val(struct val_stack** last) {
	int res = (*last)->val;
	struct val_stack* tmp = (*last)->next;
	free(*last);
	(*last) = tmp;
	return res;
}

char pop_oper(struct op_stack** last) {
	char res = (*last)->oper;
	struct op_stack* tmp = (*last)->next;
	free(*last);
	(*last) = tmp;
	return res;
}

int scan_int(FILE* iFile, char first_num) {
	if (feof(iFile))
		return first_num - '0';
	
	int tmp = first_num - '0';
	int res = tmp;
	char c  = -1;
	fscanf(iFile, "%c", &c);

	while (isdigit(c) && !feof(iFile)) {
		tmp = c - '0';
		res = res * 10 + tmp;
		fscanf(iFile, "%c", &c);
	}
	
	if (!feof(iFile))
		fseek(iFile, -1, SEEK_CUR);
	return res;
}

int identify_symb(char c) {
	if (isdigit(c)) 
		return	0;				
	if (c == '(')
		return 1;
	if (c == '*' || c == '/')
		return 2;
	if (c == '-' || c == '+')
		return 3;
	if (c == ')')
		return 4;
	if (c == '\n')
		return 5;

	return 10;
}

char oper(struct op_stack **Operation) {
	return (*Operation)->oper;
}

int count_oper(char op, int elem1, int elem2, int *dev_by_zero) {
	if (op == '+')
		return elem2 + elem1;

	if (op == '-')
		return elem2 - elem1;

	if (op == '*')
		return elem2 * elem1;

	if (op == '/') {
		if (elem1 == 0){
			*dev_by_zero = 1;
			return 0;
		}
		return int(elem2 / elem1);
	}
}

void count_oper_with_pop(struct val_stack **last_val, struct op_stack **last_op, int *dev_by_zero) {
	int elem1 = pop_val(last_val);
	int elem2 = pop_val(last_val);
	int res = count_oper(oper(last_op), elem1, elem2, dev_by_zero);
	push_val(res, last_val);
	pop_oper(last_op);
}

int first_tmp_test(int new_tmp, int *bracket_lvl) {
	if (new_tmp != 0 && new_tmp != 1) 
		return 0;
	
	if (new_tmp == 1) 
		(*bracket_lvl)++;
	
	return 1;
}

int two_tmp_test(int last_tmp, int new_tmp, int *bracket_lvl, FILE* oFile, FILE* iFile) {

	if (new_tmp == 10)
		return 0;

	if (new_tmp == 1) 
		(*bracket_lvl)++;
		
	if (new_tmp == 4 && !feof(iFile)) 
		(*bracket_lvl)--;
	
	if (*bracket_lvl < 0)
		return 0;

	if (last_tmp == 0 && new_tmp == 1)
		return 0;

	if (last_tmp == 1 && new_tmp != 1 && new_tmp != 0)
		return 0;

	if ((last_tmp == 2 || last_tmp == 3) &&
		(new_tmp == 2 || new_tmp == 3 || new_tmp == 4 || new_tmp == 5))
		return 0;

	if (last_tmp == 4 && (new_tmp == 0 || new_tmp == 1))
		return 0;

	return 1;
}

int last_tmp_test(int last_tmp, int *bracket_lvl) {
	if (last_tmp != 0 && last_tmp != 4 && last_tmp != 5)
		return 0;
	
	if (*bracket_lvl != 0) 
		return 0;
	
	return 1;
}

int input_test(FILE* iFile, FILE* oFile) {
	char new_c;
	fscanf(iFile, "%c", &new_c);
	int new_tmp = identify_symb(new_c);
	int braсket_lvl = 0;
	
	if (new_tmp == 0)
		scan_int(iFile, new_c);

	if (!first_tmp_test(new_tmp, &braсket_lvl)) 
		return 0;
	
	int last_tmp = new_tmp;

	while (!feof(iFile)) {
		fscanf(iFile, "%c", &new_c);
		new_tmp = identify_symb(new_c);
		
		if (new_tmp == 0)
			scan_int(iFile, new_c);

		if (!two_tmp_test(last_tmp, new_tmp, &braсket_lvl, oFile, iFile)) 
			return 0;
		
		last_tmp = new_tmp;
	}
	
	if (!last_tmp_test(last_tmp, &braсket_lvl)) 
		return 0;
	
	return 1;
}

int calc(FILE* iFile, FILE* oFile, struct val_stack **last_val, struct op_stack **last_op, int *dev_by_zero) {
	char new_c;
	fscanf(iFile, "%c", &new_c);

	while (!feof(iFile)) {
		int tmp = identify_symb(new_c);
		if (tmp == 0) {
			tmp = scan_int(iFile, new_c);
			push_val(tmp, last_val);
		}
		else {

			if (tmp == 3) {
				while (oper(last_op) == '*' || oper(last_op) == '/') 
					count_oper_with_pop(last_val, last_op, dev_by_zero);
				
				if (oper(last_op) == '-') 
					count_oper_with_pop(last_val, last_op, dev_by_zero);
				
				push_oper(new_c, last_op);
			}

			if (tmp == 4) {
				while (oper(last_op) != '(') 
					count_oper_with_pop(last_val, last_op, dev_by_zero);
				
				pop_oper(last_op);
			}

			if (tmp == 2) {
				if (oper(last_op) == '*' || oper(last_op) == '/')
					count_oper_with_pop(last_val, last_op, dev_by_zero);
			}

			if (tmp == 1 || tmp == 2)
				push_oper(new_c, last_op);
		}
		fscanf(iFile, "%c", &new_c);
	}

	while ((*last_op)->next && (*last_val)->next) 
		count_oper_with_pop(last_val, last_op, dev_by_zero);
	
	return (pop_val(last_val));
}


int main()
{
	struct val_stack* Val = Construct_val(0);
	struct op_stack* Op = Construct_op('+');
	FILE* iFile;
	FILE* oFile;
	iFile = fopen("in.txt", "r");
	oFile = fopen("out.txt", "w");
	if (!input_test(iFile, oFile)) {
		fprintf(oFile, "syntax error");
		return 0;
	}
	fseek(iFile, 0, SEEK_SET);

	int dev_by_zero = 0;

	int res = calc(iFile, oFile, &Val, &Op, &dev_by_zero);
	if (dev_by_zero == 1) {
		fprintf(oFile, "division by zero");
		fclose(oFile);
		fclose(iFile);
		return 0;
	}

	fprintf(oFile, "%d", res);

	fclose(oFile);
	fclose(iFile);
    return 0;
}

