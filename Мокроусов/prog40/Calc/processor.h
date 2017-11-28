#pragma once
#include <stdio.h>

typedef struct {
	int operation;
	double number;
	FILE *input, *output;
	char *string;
} TEvaluator;

int nextSymbol(FILE *fin);
double expr(FILE *fin);
double numpar(FILE *fin);
double term(FILE *fin);
void TEvaluator_GetInput(TEvaluator *evaluator);
int TEvaluator_Check(TEvaluator *evaluator);
bool isspace(char ch);
bool isdigit(char ch);