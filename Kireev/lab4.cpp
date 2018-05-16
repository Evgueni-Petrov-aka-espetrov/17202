#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct numberStack {
	uint32_t val;
	struct numberStack* prev = NULL;
}numberStack;

numberStack* numberStackConstructor(const int val, numberStack* prev) {
	numberStack* p = (numberStack*)malloc(sizeof(numberStack));
	p->val = val;
	p->prev = prev;
	return p;
}

uint32_t popNum(numberStack** st) {
	uint32_t val = (*st)->val;
	numberStack* tmp = (*st)->prev;
	free(*st);
	*st = tmp;
	return val;
}

void numberStackDestructor(numberStack* p) {
	if(p->prev!=NULL)
		numberStackDestructor(p->prev);
	free(p);
}

typedef struct operationStackPair {
	uint8_t val;
	uint8_t priority;
}operationStackPair;

uint8_t getPriority(uint8_t val) {
	switch (val){
	case '(':
		return 0;
		break;
	case '*':
		return 1;
		break;
	case '/':
		return 1;
		break;
	case '+':
		return 2;
		break;
	case '-':
		return 2;
		break;
	case ')':
		return 3;
		break;
	default:
		return 255;
		break;
	}
}

operationStackPair makeOpStackPair(uint8_t val) {
	operationStackPair p;
	p.val = val;
	p.priority = getPriority(val);
	return p;
}

typedef struct operationStack {
	operationStackPair op;
	struct operationStack* prev = NULL;
}operationStack;

operationStack* operationStackConstructor(const operationStackPair val, operationStack* prev) {
	operationStack* p = (operationStack*)malloc(sizeof(operationStack));
	p->op = val;
	p->prev = prev;
	return p;
}

operationStackPair popOp(operationStack** st) {
	operationStackPair val = (*st)->op;
	operationStack* tmp = (*st)->prev;
	free(*st);
	*st = tmp;
	return val;
}

void opStackDestructor(operationStack* p) {
	if (p->prev != NULL)
		opStackDestructor(p->prev);
	free(p);
}

// if num returns 1, else 0
int isNumber(const char ch) {
	const char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int i = 0; i < 10; ++i) {
		if (digit[i] == ch) {
			return 1;
		}
	}
	return 0;
}

int isOp(const char ch) {
	const char op[] = { '*', '/', '+', '-', '(', ')'};
	for (int i = 0; i < 6; ++i) {
		if (op[i] == ch) {
			return 1;
		}
	}
	return 0;
}

numberStack* pushNumToStack(FILE* in, numberStack* numStack) {
	fseek(in, -1, SEEK_CUR);
	int tmp;
	fscanf(in, "%d", &tmp);
	return numberStackConstructor(tmp, numStack);
}

uint32_t doOperation(uint8_t op, uint32_t leftNum, uint32_t rightNum) {
	switch (op) {
	case '*':
		return leftNum * rightNum;
		break;
	case '/':
		return leftNum / rightNum;
		break;
	case '+':
		return leftNum + rightNum;
		break;
	case '-':
		return leftNum - rightNum;
		break;
	}
	return 1;
}

void printStacks(numberStack *numStack, operationStack *opStack) {
	numberStack* tmpNumSt = numStack;
	printf("\n--------------\n");
	while (tmpNumSt != NULL) {
		printf("%d ", tmpNumSt->val);
		tmpNumSt = tmpNumSt->prev;
	}
	printf("\n");
	operationStack* tmpOpSt = opStack;
	while (tmpOpSt != NULL) {
		printf("%c ", tmpOpSt->op.val);
		tmpOpSt = tmpOpSt->prev;
	}
}

int pushOpToStack(operationStackPair newOp, numberStack **numStack, operationStack **opStack) {
	if (*opStack == NULL || (*opStack)->op.priority > newOp.priority) {
		*opStack = operationStackConstructor(newOp, *opStack);
		return 1;
	}
	while (*opStack != NULL && *numStack != NULL && (*opStack)->op.priority <= newOp.priority 
		&& (*opStack)->op.val != '(') 
	{
		uint32_t rightNum = popNum(numStack);
		uint32_t leftNum = popNum(numStack);
		operationStackPair op = popOp(opStack);
		if (op.val == '/' && rightNum == 0) {
			printf("division by zero");
			return 0;
		}
		*numStack = numberStackConstructor(doOperation(op.val, leftNum, rightNum), (*numStack));
	}
	if (newOp.val == ')') {
		popOp(opStack);
	}
	else {
		*opStack = operationStackConstructor(newOp, *opStack);
	}
}

int isSyntaxErr(const uint8_t tmpCh, const uint8_t  prevCh) {
	if (isNumber(prevCh)){
		if (tmpCh == '(')
			return 1;
	}
	else {
		if (!isOp(prevCh))
			return 1;
		if (prevCh == '(' && isOp(tmpCh) && tmpCh != '(') {
			return 1;
		}
		if (prevCh != '(' && prevCh != ')' && isOp(tmpCh) && tmpCh != '(') {
			return 1;
		}
		if (prevCh == ')' && (isNumber(tmpCh) || tmpCh == '(')) {
			return 1;
		}	
	}
	return 0;
}

int calc(FILE* in, numberStack** numStack, operationStack** opStack) {
	pushOpToStack(makeOpStackPair('('), numStack, opStack);
	uint8_t prevCh = '(';
	while (1)
	{
		uint8_t tmpCh = getc(in);
		if (feof(in)) break;
		if (tmpCh == '\n') continue;
		if (isSyntaxErr(tmpCh, prevCh)) {
			printf("syntax error");
			return 0;
		}
		prevCh = tmpCh;
		if (isOp(tmpCh)) {
			if (!pushOpToStack(makeOpStackPair(tmpCh), numStack, opStack))
				return 0;
		}
		else {
			if (isNumber(tmpCh))
				*numStack = pushNumToStack(in, *numStack);
			else {
				printf("syntax error");
				return 0;
			}
		}
	}
	if (isSyntaxErr(')', prevCh)) {
		printf("syntax error");
		return 0;
	}
	if(!pushOpToStack(makeOpStackPair(')'), numStack, opStack))
		return 0;
	printf("%d", (*numStack)->val);
	return 1;
}

int isBadInput(FILE* in) {
	int bracketsCheck = 0;
	while (1) {
		uint8_t tmpCh = getc(in);
		if (feof(in)) break;
		if (tmpCh == '\n') continue;
		if (tmpCh == '(') bracketsCheck++;
		if (tmpCh == ')') bracketsCheck--;
	}
	if (bracketsCheck != 0) {
		printf("syntax error");
		return 1;
	}
	fseek(in, 0, SEEK_SET);
	return 0;
}

int main() {
	FILE* in = fopen("in.txt", "r");
	numberStack* numStack = NULL;
	operationStack* opStack = NULL;
	if(!isBadInput(in))
		calc(in, &numStack, &opStack);
	fclose(in);

	return 0;
}
