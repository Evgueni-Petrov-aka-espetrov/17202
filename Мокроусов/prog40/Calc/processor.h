#pragma once
#include <stdio.h>

typedef struct {
	int operation;
	double number;
	FILE *input, *output;
	char *string;
} TEvaluator;

void TEvaluator_GetNextSymbol(TEvaluator *evaluator);
double TEvaluator_ProcessSumSub(TEvaluator *evaluator);
double TEvaluator_ProcessNumBrackets(TEvaluator *evaluator);
double TEvaluator_ProcessMulDiv(TEvaluator *evaluator);
void TEvaluator_GetInput(TEvaluator *evaluator);
int TEvaluator_Check(TEvaluator *evaluator);
bool isspace(char ch);
bool isdigit(char ch);