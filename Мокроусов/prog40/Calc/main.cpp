
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h> 
#include <ctype.h> 
#include "processor.h" 

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
	//Проверить вход, при неверных данных закрыться
	if (!TEvaluator_Check(&evaluator)) {
		fprintf(evaluator.output, "syntax error");
		free(evaluator.string);
		return 0;
	}
	fseek(evaluator.input, 0, SEEK_SET);
	TEvaluator_GetNextSymbol(&evaluator);
	while (evaluator.operation) {
		double exp = TEvaluator_ProcessSumSub(&evaluator);
		//Если получилось деление на ноль, сообщаем
		if (isinf(exp) || isnan(exp)) {
			fprintf(evaluator.output, "division by zero");
			fclose(evaluator.output);
			fclose(evaluator.input);
			return 0;
		}

		int exp_int = (int)exp;
		fprintf(evaluator.output, "%d", exp_int);
		//Выводим ответ
	}
	fclose(evaluator.input);
	fclose(evaluator.output);
	return 0;
}