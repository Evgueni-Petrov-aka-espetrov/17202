#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read(char* string, FILE* iFile) {
	char tmp = (char)fgetc(iFile);
	int i = -1;
	while (tmp != '\n' && i < 9) {
		i++;
		string[i] = tmp;
		tmp = (char)fgetc(iFile);
	}
	string[i + 1] = '\0';
}


int char_to_int(char ch){
	char digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int i = 0; i < 10; i++) {
		if (digits[i] == ch)
			return i;
	}
	return -1;
}


int input_test(char* string) {
	int tmp[10] = { 0 };
	for (unsigned int i = 0; i < strlen(string); i++) {
		if (char_to_int(string[i]) == -1)
			return 1;
		tmp[char_to_int(string[i])] += 1;
		if (tmp[char_to_int(string[i])] > 1 )
			return 1;
	}
	return 0;

}

void swap(char* a1, char* a2) {
	char tmp = *a1;
	*a1 = *a2;
	*a2 = tmp;
}

void reverse(char* string, int position) {
	int j = strlen(string) - 1;
	int i = position;
	
	while (j > i) {
		swap(&string[i], &string[j]);
		j--;
		i++;
	}
}

char* gen_next_reshuffle(char* string) {
	int i = 0;
	int t = 0;
	for (i = strlen(string) - 2; i >= 0; i--) {
		if (string[i] < string[i + 1]) {
			t = 1;
			break;
		}
	}
	if (t == 0)
		return NULL;
	int j;
	int tmp;
	char min = '9';
	for (j = strlen(string) - 1; j >= i; j--){
		if (string[j] > string[i] && string[j] <= min) {
			min = string[j];
			tmp = j;
		}			
	}

	swap(&string[i], &string[tmp]);
	reverse(string, i + 1);
	return string;
}

void gen_reshuffles(char* string, int n, FILE* oFile) {
	for (int i = 0; i < n; i++) {
		string = gen_next_reshuffle(string);
		if (string == NULL)
			break;

		fprintf(oFile, "%s\n", string);
	}
}


int main() {
	FILE *iFile = fopen("in.txt", "r");
	FILE *oFile = fopen("out.txt", "w");
	int n;
	char string[11];
	read(string, iFile);
	
	fscanf(iFile, "%d", &n);
	if (input_test(string) == 1) {
		fprintf(oFile, "bad input");
		return 0;
	}
	
	gen_reshuffles(string, n, oFile);

	fclose(iFile);
	fclose(oFile);

	return 0;
}