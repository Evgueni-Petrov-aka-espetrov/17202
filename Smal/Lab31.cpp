
#include <stdio.h>
#include <stdlib.h>


void quicksort( int* A,const int size,const FILE* oFile) {
	int hi = size - 1;
	int lo = 0;
	//print(A, oFile, size);
	int p = A[size / 2 ];
	//fprintf(oFile, "\n%d\n", p);
	int tmp = 0;

	while (hi > lo) {
		while (A[hi] > p) hi--;
		while (A[lo] < p) lo++;
		if (hi > lo) {
			if (A[hi] == A[lo]) {
				hi--;
				lo++;
				}
			else {
				//fprintf(oFile, "%d  %d\n", A[lo], A[hi]);
				tmp = A[lo];
				A[lo] = A[hi];
				A[hi] = tmp;
			}
			
			
			

		}
	} while (hi > lo);
	
	if (hi > 0) quicksort(A , hi, oFile);
	if (size > lo + 1) quicksort(A + lo, size - lo, oFile);

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

		for (int i = 0; i < size; i++) {
			fscanf(iFile, "%d", &A[i]);
		}

		quicksort(A, size, oFile);

		for (int i = 0; i < size; i++)
				fprintf(oFile, "%d ", A[i]);
		
		fclose(iFile);
		fclose(oFile);
		free(A);
		return 0;
	
	
	/*
	FILE *iFile;
	iFile = fopen("in.txt", "r");
	int size = 0;
	fscanf(iFile, "%d", &size);

	int* A = (int*)malloc(sizeof(int)*size);
	int i = 0;
	for (i = 0; i < size; i++)
		fscanf(iFile, "%d ", A[i]);
	fclose(iFile);

	quicksort(A, size);

	FILE *oFile;
	oFile = fopen("out.txt", "w");
	for (i = 0; i < size; i++)
		fprintf(oFile, "%d ", A[i]);

	fclose(oFile);
	free(A);
    return 0;*/
}

