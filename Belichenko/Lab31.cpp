// Lab31.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"


void swap(int* p, int i, int j) {
	int a = p[i];
	p[i] = p[j];
	p[j] = a;
}

void quicksort(int *arr, int N) {
	int i = 0;
	int j = N-1;
	int pivot = arr[N/2];
	while(i <= j) {
		while (arr[i] < pivot)	i++;
		while (arr[j] > pivot)	j--;
		if (i <= j) {
			swap(arr, i, j);
			i++;
			j--;
		}
	} 
	if (j > 0)	quicksort(arr, j+1);
	if (i < N)	quicksort(&arr[i], N-i);
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
	int N = 0;
	fscanf(fin, "%d", &N);
	int* arr = (int*)calloc(N, sizeof(int));
	for (int i = 0; i < N; i++)
		fscanf(fin, "%d", &arr[i]);

	quicksort(arr, N);
	for (int i = 0; i < N; i++)
		fprintf(fout, "%d ", arr[i]);
	free(arr);
	fclose(fin);
	fclose(fout);
	return 0;
}

