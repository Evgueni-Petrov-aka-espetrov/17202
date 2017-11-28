
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h> 
#include <ctype.h> 
#include "processor.h" 

extern int operation;
extern double number;

int main() {
	TEvaluator evaluator;
	//Открыть входной файл, при ошибке завершиться
	evaluator.input = fopen("in.txt", "r");
	if (!evaluator.input)
	{
		printf("ERROR: No input file found!\n");
		return 0;
	}
	//Открыть выходной файл, при ошибке завершиться
	evaluator.output = fopen("out.txt", "w");
	if (!evaluator.output)
	{
		fclose(evaluator.input);
		printf("ERROR: No output file found!\n");
		return 0;
	}	
	//Считать входную строку
	TEvaluator_GetInput(&evaluator);
	if (TEvaluator_Check(&evaluator)) {
		fprintf(evaluator.output, "syntax error");
		free(evaluator.string);
		return 0;
	}

	fclose(evaluator.input);
	evaluator.input = fopen("in.txt", "r");
	nextSymbol(evaluator.input);
	while (operation != EOF) {
		double exp = expr(evaluator.input);
		//Checking division by zero and infinity 
		if (isinf(exp) == 1 || isnan(exp) == 1) {
			fprintf(evaluator.output, "division by zero");
			fclose(evaluator.output);
			fclose(evaluator.input);
			return 0;
		}

		int exp_int = (int)exp;
		fprintf(evaluator.output, "%d", exp_int);
	}
	fclose(evaluator.input);
	fclose(evaluator.output);
	return 0;
}