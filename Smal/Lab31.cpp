#include <stdio.h>
#include <stdlib.h>

void quicksort(int* A, int size) {
	int hi = size - 1;
	int lo = 0;
	int p = A[size / 2];
	while (hi > lo) {
		while (A[hi] > p) --hi;
		while (A[lo] < p) ++lo;
		if (hi > lo) {
			if (A[hi] == A[lo]) {
				hi--;
				lo++;
			} else {
				int tmp = A[lo];
				A[lo] = A[hi];
				A[hi] = tmp;
			}
		}
	} while (hi > lo);
	
	if (hi > 0) quicksort(A , hi);
	if (size > lo + 1) quicksort(A + lo, size - lo);
}

int main()
{	
	FILE *iFile;
	FILE *oFile;
	iFile = fopen("in.txt", "r");
	oFile = fopen("out.txt", "w");
	int size;
	fscanf(iFile, "%d", &size);
	int* A = (int*)malloc(sizeof(int)*size);
	if (A == NULL) {
		printf("out of memory\n");
		return 1;
	}

	for (int i = 0; i < size; i++) {
		fscanf(iFile, "%d", &A[i]);
	}

	quicksort(A, size);

	for (int i = 0; i < size; i++) {
		fprintf(oFile, "%d ", A[i]);
	}
		
	fclose(iFile);
	fclose(oFile);
	free(A);
	return 0;
}

