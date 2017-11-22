#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//установка стоп символов
void set_stop_symb( char* model, int length, int* stop_symb, FILE* oFile) {
	int i;
	char dop_char;
	char symb;
	dop_char = *(model + length - 1);
	*(model + length - 1) = 0;
	for (i = 0; i < length - 1; i++) {
		symb = *(model + i);
		*(stop_symb + i) = strrchr(model, symb) - model + 1;
	}
	*(model + length - 1) = dop_char;
	*(stop_symb + i) = strlen(model);
}

//Поиск стоп-символа
int finding_stop(char symb, int* arr_of_stop, char* model) {
	for (int i = 0;  i < strlen(model); i++) {
		if (model[i] == symb)
			return arr_of_stop[i];
	}
	return 0;
}

//основное сравнение
void compare_full(int *arr_of_stop, char *model, char *string, FILE* oFile) {
	char last_symb = *(model + strlen(model) - 1); 
	int i;
	for (i = strlen(model); i < strlen(string);) {
		fprintf(oFile, "%d ", i);
		if (last_symb == *(string + i)) {
			i += compare_model(i, model, string, arr_of_stop, oFile);		
		}
		else {
			i = i + strlen(model) - finding_stop(*(string + i), arr_of_stop, model);
		}	
	}
	
}

//Сравнение, если совпал последний символ
int compare_model(int i,char *model, char *string, int *arr_of_stop, FILE* oFile) {
	char first_symb = *(string + i);
	int j = strlen(model)-1;
	while ((*(string + i) == *(model + j)) && (j != 0)) {
		i--;
		fprintf(oFile, "%d ", i);
		j--;
	}
	if (j == 0) {											//Если найдено полное вхождение подстроки в строку
		return strlen(model);
	}

	else{ 
		return strlen(model) - finding_stop(first_symb, arr_of_stop, model);
	}
}

int main() {

	FILE *iFile = fopen("in.txt", "r");
	FILE *oFile = fopen("out.txt", "w");
	char model[18] = { 0 };
	int i = 0;
	char c;
	
	fgets(model, 18, iFile);
	model[strlen(model) - 1] = '\0';
	int len_model = strlen(model);
	int stop_symb[18] = { 0 };
	set_stop_symb(model, len_model, stop_symb, oFile);
	fseek(iFile, 0, SEEK_END);
	int len_string = ftell(iFile);
	len_string -= strlen(model);
	fseek(iFile, strlen(model), SEEK_SET);
	char* string =(char*) malloc(sizeof(char) * len_string);
	for (i = 0; i < len_string - 1; i++) {
		string[i] = fgetc(iFile);
	}
	string[i] = '\0';
	
	compare_full(stop_symb, model, string, oFile);
	fclose(iFile);
	fclose(oFile);

	return 0;
}