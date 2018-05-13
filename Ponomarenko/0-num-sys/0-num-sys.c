#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define BUFFER_SIZE 256
int int_from_char(char s) {//перевод из сивола в целое число
	switch (s) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a':
		case 'A': return 10;
		case 'b':
		case 'B': return 11;
		case 'c':
		case 'C': return 12;
		case 'd':
		case 'D': return 13;
		case 'e':
		case 'E': return 14;
		case 'f':
		case 'F': return 15;
	}
}
char char_from_int(int s) {//перевод из числа в символ
	switch (s) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
		default: return '0';
	}
}
long long int transfer10int(char* s, int cc) {//получение массива из чисел целой части
	long long int trans10int = 0;
	int i = 0;
	while ((s[i] != '.') && (s[i] != '\0')) { i++; }

	if (s[0] == '.') {
		printf("Bad input");
		exit(0);
	}

	for (int j = i - 1; j >= 0; j--) {
		if (int_from_char(s[j]) < cc) {
			trans10int += int_from_char(s[j]) * pow(cc, i - j - 1); }
		else { 
			printf("Bad input");
			exit(0);	
		}
	}

	return trans10int;
}

double transfer10real(char* s, int cc) {//получение массива из чисел целой части
	double trans10real = 0;
	int i = 0;
	while ((s[i] != '.') ) {
		if (s[i] == '\0') {
			return 0.0;
		}
		i++;
	}


	if (s[i+1] == '\0') {
		printf("Bad input");
		exit(0);
	}

	for (int j = i + 1; s[j] != '\0'; j++) {
		if (int_from_char(s[j]) < cc) {
			trans10real += int_from_char(s[j]) * pow(cc, i - j);
		}
		else { 
			printf("Bad input"); 
			exit(0);
		}
	}

	return trans10real;
}


void transferccinteger(long long int transfer10int,int cc) {//перевод и вывод целой части числа
		int finalint[BUFFER_SIZE] = { 0 };//обнуление массива
		int i = 0;
		
		while (transfer10int > 0) {//вычисление остатков от деления
			finalint[i] = transfer10int % cc;
			i++;
			transfer10int /= cc;
		}
		int firstmean = 0;
		
		for (i = BUFFER_SIZE - 1; i >= 0; i--) {
			if ((finalint[i] == 0) && (firstmean == 0)) { //пропускаем не заполненные элементы масства(числа)
				if (i == 0) { printf("%c", char_from_int(finalint[i] )); }
				continue; 
			}
			else { //выводим целую часть числа
				firstmean = 1; 
				printf("%c",char_from_int(finalint[i])); 
			}
		}
	}

void transferccreal(double transfer10real, int cc) {
		int finalreal;

		while (transfer10real > 0) {
			transfer10real *= cc;
			finalreal =(int) (transfer10real);//вычисление цифры
			transfer10real -= finalreal;
		    printf("%c", char_from_int(finalreal));//вывод
		}
		
	}

int main() {

	int cc1, cc2;//исходная и конечная системы счисления

		fprintf(stderr, "Write first and second ");
		scanf("%d %d", &cc1, &cc2);
		if (!((cc1 <= 16) && (cc2 <= 16) && (cc1 >= 2) && (cc2 >= 2))) {
			printf("Bad input");
			exit(0);

		};

	char number_as_string[BUFFER_SIZE];
	fprintf(stderr, "Write a number ");
	scanf("%s", &number_as_string);//считывание числа (в виде строки)
	
	long long int int10;
	double real10;
	int10 = transfer10int(number_as_string, cc1);
	real10 = transfer10real(number_as_string, cc1);
	
	transferccinteger(int10, cc2);
	printf(".");
	transferccreal(real10, cc2);
	printf("\n");
	
	return 0;
}
