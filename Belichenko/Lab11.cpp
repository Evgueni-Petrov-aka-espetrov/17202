// Lab11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <locale>

int my_pow(int i) {
	switch (i) {
	case 0: return 1;
	case 1: return 3;
	case 2: return 9;
	case 3: return 27;
	case 4: return 81;
	case 5: return 243;
	case 6: return 729;
	case 7: return 2187;
	case 8: return 6561;
	case 9: return 19683;
	case 10: return 59049;
	case 11: return 177147;
	case 12: return 531441;
	case 13: return 1594323;
	case 14:return 4782969;
	case 15: return 14348907;
	}
}


int get_hash(char* str, int N, int prevhash) {
	int hash = 0;
	if (prevhash == -1) {
		for (int i = 0; i < N; i++) {
			int c = str[i];
			if (str[i] == '\n') c = 3;
			if (c < 0) c += 127;
			hash += c % 3 * my_pow(i);
		}
		return hash;
	}
	int a = str[0];
	int b = str[N-1];
	if (a < 0) a += 127;
	if (b < 0) b += 127;
	if (str[0] == '\n') a = 3;
	if (str[N-1] == '\n') b = 3;
	hash = (prevhash - (a%3)) / 3 + (b % 3 * my_pow(N-1));
	return hash;
}

void check(FILE* fin, FILE* fout, int text, char* pattern, int N) {
	fseek(fin, -N, SEEK_CUR);
	char c;
	for (int i = 0; i < N; i++) {
		fprintf(fout, " %d", text - N + i +1);
		c = fgetc(fin);
		if (c != pattern[i]) {
			fseek(fin, N - i - 1, SEEK_CUR);
			break;
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
	int phash = get_hash(pattern, N, -1);
	fprintf(fout, "%d", phash);

	c = fgetc(fin);
	int text = 1;
	int hash = -1, checked = 0;
	int fix = 0, count = 0; // нужны. чтобы изолировать баг с пропуском символа, стоящим на определенном расстоянии после перевода строки
	char* curstr = (char*)calloc(N, sizeof(char));
	while (c != EOF && checked != N) {
		curstr[checked] = c;
		checked++;
		c = fgetc(fin);
		text++;
		if (c == '\n') fix = 1;
		if (checked == N)	hash = get_hash(curstr, N, hash);

		if (hash == phash) {
			if(c!=EOF) fseek(fin, -1, SEEK_CUR);
			if (c == '\n') fseek(fin, -1, SEEK_CUR);
			check(fin, fout, text-1, pattern, N);
			if (c != EOF) fseek(fin, 1, SEEK_CUR);
			if (c == '\n') fseek(fin, 1, SEEK_CUR);
		}
	}
	while (c != EOF) {
		curstr[N - 1] = c;
		hash = get_hash(curstr, N, hash);
		fseek(fin, -N, SEEK_CUR);
		if (fix)	fseek(fin, -1, SEEK_CUR);
		curstr[0] = fgetc(fin);
		if (fix)	fseek(fin, 1, SEEK_CUR);
		fseek(fin, N-1, SEEK_CUR);
		if (hash == phash) {
			if (count == N - 1 && fix == 1)	fseek(fin, -1, SEEK_CUR);
			check(fin, fout, text, pattern, N);
			c = fgetc(fin);
		}
		if (count == N - 2 && fix == 1)	fseek(fin, -1, SEEK_CUR);
		c = fgetc(fin);
		if (count == N - 2 && fix == 1) fseek(fin, 1, SEEK_CUR);
	//	if (count == N+1) count = -1;
		count++;
		text++;
	}
	free(curstr);
	free(pattern);
	fclose(fin);
	fclose(fout);
	return 0;
}


