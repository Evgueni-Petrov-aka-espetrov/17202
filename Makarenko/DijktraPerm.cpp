#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

enum {MaxSize = 10};

struct arr{
	int counter;
	int *massive;
};

void swap(int* a, int* b) {
	int buffer;
	buffer = *a;
	*a = *b;
	*b = buffer;
}
char getNextChar(FILE* input) {
	char tmp;
	fscanf(input, "%c", &tmp);
	return tmp;
}
int charToInt(char c) {
	return c - '0';
}

arr* reading(FILE* input) {
	int lenght = 0;
	arr* dp = (arr*)malloc(sizeof(arr));
	dp->massive = (int*)malloc(sizeof(int)*MaxSize);

	while (lenght < MaxSize){
		char c = getNextChar(input);
		if (c == '\n') {
			break;
		}
		dp->massive[lenght] = charToInt(c);
		lenght++;
	}
	dp->counter = lenght;
	return dp;
}

void printing(FILE* output, arr* dp) {
	for (int i = 0; i < dp->counter; i++)
	{
		fprintf(output, "%d", dp->massive[i]);
	}
	fprintf(output, "\n");

}
int checkingf(arr* dp) {
	for (int i = 0; i < dp->counter; i++) {
		if ((dp->massive[i] > 9) || (dp->massive[i] < 0)) {
			return 1;
		}
	}
	return 0;
}

int checkings(arr* dp) {
	int check[10] = {};
	for (int i = 0; i < dp->counter; i++) {
		++check[dp->massive[i]];
	}
	for (int i = 0; i < 10; i++){
		if (check[i] > 2) {
			return 1;
		}
	}
	return 0;
}

int dijktra(arr* dp) {
	int pointer = dp->counter - 2;
	while ((dp->massive[pointer] > dp->massive[pointer + 1]) && pointer != -1) {
		--pointer;
	}
	if (pointer == -1) {
		return 0;
	}
	for (int i = dp->counter - 1; i > pointer; i--) {
		if (dp->massive[i] > dp->massive[pointer]) {
			swap(&dp->massive[i], &dp->massive[pointer]);
			break;
		}
	}
	for (int i = dp->counter - 1; i > pointer; i--) {
		for (int j = i; j > pointer; j--) {
			if (dp->massive[i] < dp->massive[j]) {
				swap(&dp->massive[i], &dp->massive[j]);
			}
		}
		
	}
	return 1;
}

	int main() {
		FILE* input = fopen("in.txt", "r");
		FILE* output = fopen("out.txt", "w");
		arr* dp = reading(input);

		int counter;
		fscanf(input, "%d", &counter);

		if (checkingf(dp) == 1) {
			fprintf(output, "bad input");
			return 0;
		}
		if (checkings(dp) == 1) {
			fprintf(output, "bad input");
			return 0;
		}

		for (int i = 0; i < counter; i++){
			if (dijktra(dp) == 0) {
				break;
			}
			else{
				printing(output, dp);
			}
		}
		return 0;
	}
