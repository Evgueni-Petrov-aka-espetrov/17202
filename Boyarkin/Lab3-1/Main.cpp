#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable : 4996)

void quicksort(int *arr, int size);
int* partition(int *left, int *right);
void swap(int *lhs, int *rhs);

const int BUF_SIZE = 100000;

int main() {

	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	if (in == NULL) {
		fprintf(out, "Couldn't open file\n");
		goto END;
	}

	int *arr;
	int n;

	fscanf(in, "%d", &n);
	if (n < 0) {
		fprintf(out, "n is negative!\n");
		goto END;
	}

	arr = (int*)malloc(n * sizeof(*arr));
	if (arr == NULL) {
		fprintf(out, "out of memory!\n");
		goto END;
	}

	for (int i = 0; i < n; ++i) {
		fscanf(in, "%d", &arr[i]);
	}

	quicksort(arr, n);

	setvbuf(out, NULL, _IOFBF, BUF_SIZE);
	for (int i = 0; i < n; ++i) {
		fprintf(out, "%d ", arr[i]);
		if (i % 1024 == 0) {
			fflush(out);
		}
	}

END:
	if (in != NULL) fclose(in);
	fclose(out);
	return 0;
}

void quicksort(int *arr, int size) {
	if (size <= 1) return;

	int *part = partition(arr, arr + size - 1) + 1;
	quicksort(arr, part - arr);
	quicksort(part, size - (part - arr));
}

int* partition(int *left, int *right) {
	int pivot = *left;

	--left;
	++right;
	for (;;) {
		do ++left;
		while (*left < pivot);

		do --right;
		while (*right > pivot);

		if (right <= left) return right;
		swap(left, right);
	}
}

void swap(int *lhs, int *rhs) {
	if (lhs == rhs) return;

	*lhs ^= *rhs;
	*rhs ^= *lhs;
	*lhs ^= *rhs;
}
