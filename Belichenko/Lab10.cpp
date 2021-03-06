// Lab1-0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"


void GenerateStopTable(char* pattern, int* stoptable, int N) {
	for (int i = 0; i < 128; i++) {
		stoptable[i] = N;
	}
	for (int i = 0; i < N; i++) {
		for (int j = N - 2; j >= i; j--) {
			if (pattern[j] == pattern[i]) {
				stoptable[(int)pattern[j]] = N-1-j;
				break;
			}
		}
	}
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "wt");
	if (fin == NULL) {
		fprintf(fout, "bad input");
		fclose(fout);
		return 0;
	}
	char* pattern = (char*)calloc(16, sizeof(char));
	char c;
	int N = 0;
	c = fgetc(fin);
	while (c != '\n') {
		pattern[N] = c;
		N++;
		c = fgetc(fin);
	}
	int* stoptable = (int*)calloc(128, sizeof(int));
	GenerateStopTable(pattern, stoptable, N);

	fseek(fin, N-1, SEEK_CUR);
	int cpos = -10;
	int shift = -10;
	c = fgetc(fin);
	int text = N;
	int limit = 0;
	char endPos = c;
	while (c != EOF) {
		for (int i = N - 1; i >= 0; i--) {
			fprintf(fout, "%d ", text);
			if (i == N-1) 
				endPos = c;
			if (c == pattern[i]) {
				text--;
				fseek(fin, -2, SEEK_CUR);
				c = fgetc(fin);
				if (i == 0) {
					fseek(fin, 2*N-1, SEEK_CUR);
					text += 2*N-1;
				}
			}
			else {
				shift = stoptable[endPos];
				fseek(fin, shift - 2 + N-i, SEEK_CUR);
				text += shift - 2 + N-i;
				break;
			}
		}
		c = fgetc(fin);
		text++;
	}
	free(pattern);
	free(stoptable);
	fclose(fin);
	fclose(fout);
	return 0;
}



