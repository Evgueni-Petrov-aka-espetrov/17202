#include "processor.h"
#include <string.h>
#include <stdlib.h> 


//read one symbol from file and returns number or operation, and also check it 
void TEvaluator_GetNextSymbol(TEvaluator *evaluator) {
	for (;;) {
		//Get symbol 
		int char_tmp = fgetc(evaluator->input);
		//If the symbol is one of the operation(also the end of the file), return it 
		switch (char_tmp)
		{
		case EOF: evaluator->operation = '\0'; return;
		case '+': case '-': case '*': case '/': case '(': case ')': evaluator->operation = char_tmp; return;
		}
		//If symbol is space we just skip this iteration 
		if (isspace(char_tmp)) continue;
		// If the symbol is number, we gets whole number and return it 
		if (isdigit(char_tmp)) {
			//Return the line stream back 
			ungetc(char_tmp, evaluator->input);
			//And we read the whole number 
			fscanf(evaluator->input, " %lf", &(evaluator->number));
			evaluator->operation = 'D';
			return;
		}
	}
}


//Declare a function for later use 


// Возвращает число или обрабатывает выражение в скобках
double TEvaluator_ProcessNumBrackets(TEvaluator *evaluator){
	double t;
	if (evaluator->operation == 'D') { //число
		t = evaluator->number;
		TEvaluator_GetNextSymbol(evaluator);
	}
	else //выражение в скобках
	{
		TEvaluator_GetNextSymbol(evaluator);
		t = TEvaluator_ProcessSumSub(evaluator);
		TEvaluator_GetNextSymbol(evaluator);
	}	
	return t;
}


double TEvaluator_ProcessMulDiv(TEvaluator *evaluator) {
	double t = TEvaluator_ProcessNumBrackets(evaluator);
	while (1) {
		switch (evaluator->operation)
		{
		case '*': TEvaluator_GetNextSymbol(evaluator); t *= TEvaluator_ProcessNumBrackets(evaluator); break;
		case '/': TEvaluator_GetNextSymbol(evaluator); t /= TEvaluator_ProcessNumBrackets(evaluator); break;
		default: return t;
		}
	}
}

double TEvaluator_ProcessSumSub(TEvaluator *evaluator) {
	double t = TEvaluator_ProcessMulDiv(evaluator);
	while (1) {
		switch (evaluator->operation)
		{
		case '+': TEvaluator_GetNextSymbol(evaluator); t += TEvaluator_ProcessMulDiv(evaluator); break;
		case '-': TEvaluator_GetNextSymbol(evaluator); t -= TEvaluator_ProcessMulDiv(evaluator); break;
		default: return t;
		}
	}
}
// Считать входную строку
void TEvaluator_GetInput(TEvaluator *evaluator)
{
	char *temp = (char*)0;
	int count_read = 0;
	int read_ch = 0;
	char *values = (char*)0;
	while (1) {

		int read_ch = fgetc(evaluator->input);
		if (read_ch == EOF) break;
		//Если считался не нулевой символ, добавляем его в буфер
		count_read++;
		temp = (char *)realloc(values, (count_read)* sizeof(char));
		values = temp;
		values[count_read - 1] = (char)read_ch;
	}
	//Вставить символ конца строки
	temp = (char *)realloc(values, (count_read + 1)* sizeof(char));
	values = temp;
	values[count_read] = '\0';
	count_read++;	
	if ((values[count_read - 2] == ' ') || (values[count_read - 2] == '\n'))
		values[count_read - 2] = 0;
	evaluator->string = values;
}

int TEvaluator_Check(TEvaluator *evaluator) {
	char *temp = evaluator->string;
	int was_num = 0;
	int opened_brak = 0;
	//Пока входная строка не окончилась
	while (*temp)
	{
		//Проверяем отсутствие недопустимых символов во входе
		if (!((*temp >= '0' && *temp <= '9') || (*temp == '+') || (*temp == '-') || (*temp == '/') || (*temp == '*') || (*temp == '(') || (*temp == ')')))
			return 0;
		if (*temp >= '0' && *temp <= '9')
			was_num = 1;
		//Проверяем невозможные комбинации символов
		if (*temp == '(') {
			opened_brak++;
			if (*(temp + 1))
				switch (*(temp + 1))
			{
				case '+': case '/': case '*': case ')': return 0;
			}
		}
		if (*temp == ')')
		{
			if (opened_brak == 0) return 0;
			opened_brak--;
		}
		if ((*temp == '+') || (*temp == '-'))
		{
			switch (*(temp + 1))
			{
			case 0: case '+': case '-': case '*': case '/': case ')': return 0;
			};
		}
		if ((*temp == '*') || (*temp == '/'))
		{
			switch (*(temp + 1))
			{
			case 0: case '+': case '*': case '/': case ')': return 0;
			};
		}
		temp++;
	}
	//Если не было ни одного числа или нарушен порядок раскрытия скобок - ошибка
	if (!was_num || (opened_brak != 0))
		return 0;
	return 1;
}


bool isspace(char ch)
{
	return ch == ' ';
}

bool isdigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}