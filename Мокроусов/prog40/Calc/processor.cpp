#include "processor.h"
#include <string.h>
#include <stdlib.h> 

int operation;
double number;

int TEvaluator_GetChar(TEvaluator *evaluator)
{
	char temp = *(evaluator->string);
	if (temp)
		return temp;
	else return EOF;
}
//read one symbol from file and returns number or operation, and also check it 
int nextSymbol(FILE *fin) {
	for (;;) {
		//Get symbol 
		int char_tmp = fgetc(fin);
		//If the symbol is one of the operation(also the end of the file), return it 
		if (char_tmp == EOF || (strchr("+-*/()", char_tmp) != NULL)) { // if the symbol is one of the operation(also the end of the file), returns it 
			operation = char_tmp;
			return operation;
		}
		//If symbol is space we just skip this iteration 
		if (isspace(char_tmp))
			continue;
		// If the symbol is number, we gets whole number and return it 
		if (isdigit(char_tmp)) {
			//Return the line stream back 
			ungetc(char_tmp, fin);
			//And we read the whole number 
			fscanf(fin, " %lf", &number);
			operation = 'n';
			return operation;
		}
	}
}


//Declare a function for later use 


// numpar returns number | '(' expr ')' 
double numpar(FILE *fin) {
	if (operation == 'n') {
		double x = number;
		nextSymbol(fin);
		return x;
	}
	nextSymbol(fin); // '(' 
	double x = expr(fin);
	nextSymbol(fin); // ')' 
	return x;
}


// term -> numpar || term '*' -> numpar || term '/' -> numpar 
double term(FILE *fin) {
	double x = numpar(fin);
	while (1) {
		if (operation == '*') {
			nextSymbol(fin); // '*' 
			x *= numpar(fin);
		}
		else if (operation == '/') {
			nextSymbol(fin); // '/' 
			x /= numpar(fin);
		}
		else return x;
	}
}

// expr -> term | expr '+' -> term | expr '-' -> term 
double expr(FILE *fin) {
	double x = term(fin);
	while (1) {
		if (operation == '+') {
			nextSymbol(fin); // '+' 
			x += term(fin);
		}
		else if (operation == '-') {
			nextSymbol(fin); // '-' 
			x -= term(fin);
		}
		else return x;
	}
}
// Just read from file and return char array 
void TEvaluator_GetInput(TEvaluator *evaluator)
{
	char *temp = (char*)0;
	int count_read = 0;
	int read_ch = 0;
	char *values = (char*)0;
	while (1) {

		//Reading one byte from a file 
		int read_ch = fgetc(evaluator->input);

		//Check for end of file 
		if (read_ch == EOF) break;

		count_read++;
		//Memory allocation for char_numbers 
		temp = (char *)realloc(values, (count_read)* sizeof(char));
		values = temp;
		values[count_read - 1] = (char)read_ch;
	}
	temp = (char *)realloc(values, (count_read + 1)* sizeof(char));
	values = temp;
	values[count_read] = '\0';
	count_read++;

	//Fixing spaces and line breaks in a file 
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
		if (!((*temp >= '0' && *temp <= '9') || (*temp == '+') || (*temp == '-') || (*temp == '/') || (*temp == '*') || (*temp == '(') || (*temp == ')')))
			return 1;
		if (*temp >= '0' && *temp <= '9')
			was_num = 1;
		if (*temp == '(') {
			opened_brak++;
			if (*(temp + 1))
				switch (*(temp + 1))
			{
				case '+': case '/': case '*': case ')': return 1;
			}
		}
		if (*temp == ')')
		{
			if (opened_brak == 0) return 1;
			opened_brak--;
		}
		if ((*temp == '+') || (*temp == '-'))
		{
			switch (*(temp + 1))
			{
			case 0: case '+': case '-': case '*': case '/': case ')': return 1;
			};
		}
		if ((*temp == '*') || (*temp == '/'))
		{
			switch (*(temp + 1))
			{
			case 0: case '+': case '*': case '/': case ')': return 1;
			};
		}
		temp++;
	}
	if (!was_num || (opened_brak != 0))
		return 1;
	return 0;
}



bool isspace(char ch)
{
	return ch == ' ';
}

bool isdigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}