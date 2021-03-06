// Lab30.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"


void swap(int* p, int i, int j) {
	int a = p[i];
	p[i] = p[j];
	p[j] = a;
}

void build_heap(int* arr, int N, int i) {
	int max = i;
	int left = 2*i + 1;
	int right = 2*i + 2; 
	if (left < N && arr[left] > arr[max])	max = left;
	if (right < N && arr[right] > arr[max])	max = right;
	if (max != i) {
		swap(arr, i, max);
		build_heap(arr, N, max);
	}
}

void heapSort(int* arr, int N) {
	for (int i = N/2 - 1; i >= 0; i--)
		build_heap(arr, N, i);

	for (int i = N-1; i >= 0; i--)  {
		swap(arr, 0, i);
		build_heap(arr, i, 0);
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
	int N = 0;
	fscanf(fin, "%d", &N);
	int* arr = (int*)calloc(N, sizeof(int));
	for (int i = 0; i < N; i++)
		fscanf(fin, "%d", &arr[i]);

	heapSort(arr, N);
	for (int i = 0; i < N; i++)
		fprintf(fout, "%d ", arr[i]);

	free(arr);
	fclose(fin);
	fclose(fout);
	return 0;
}
