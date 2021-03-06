// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"


void swap(int* p, int i, int j) {
	int a = p[i];
	p[i] = p[j];
	p[j] = a;
}


void sort(int* a, int s, int N) {
	for (int i = N - 1; i >= s; i--) {
		for (int j = s; j<i; j++) {
			if (a[j] > a[j + 1]) swap(a, j, j + 1);
		}
	}
}

int shuffle(int* perm, int N) {
	for (int i = N - 2; i >= 0; i--) {
		if (perm[i] < perm[i+1]) {
			int min = perm[i + 1];
			int b = i + 1;
			for (int j = i + 1; j < N; j++) {
				if (perm[j] < min && perm[j] > perm[i]) {
					min = perm[j];
					b = j;
				}
			}
			swap(perm, i, b);
			sort(perm, i + 1, N);
			return 1;
		}
	}
	return 0;
}



void print_permutations(FILE* fout, int M, int* perm, int N) {
	int a = 0;
	for (int i = 0; i < M; i++) {
		a = shuffle(perm, N);
		if (a == 0) return;
		for (int j = 0; j < N; j++) {
			fprintf(fout, "%d", perm[j]);
		}
		fprintf(fout, "\n");
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
	int* permutation = (int*)calloc(10, sizeof(int));
	char c;
	int N = 0;
	c = fgetc(fin);
	while (c != '\n') {
		permutation[N] = c-48;
		for (int i = 0; i < N; i++) {
			if (permutation[i] == permutation[N] || permutation[N] > 9 || permutation[N] < 0) {
				fprintf(fout, "bad input");
				free(permutation);
				fclose(fin);
				fclose(fout);
				return 0;
			}
		}
		N++;
		c = fgetc(fin);
	}
	
	int M = 0;
	fscanf(fin, "%d", &M);
	if(N>1)	print_permutations(fout, M, permutation, N);

	free(permutation);
	fclose(fin);
	fclose(fout);
	return 0;
}



