#include <stdio.h>
#include <stdlib.h>



void swap(int* a1, int* a2) {
	int tmp = *a1;
	*a1 = *a2;
	*a2 = tmp;
}

void heapify(int* arr, int size, int i) {
	int left;
	int right;
	int max;

	while (1) {
		left = i * 2 + 1;
		right = left + 1;
		max = i;

		if ((left < size) && (arr[left] > arr[max]))
			max = left;

		if ((right < size) && (arr[right] > arr[max]))
			max = right;

		if (max == i)
			break;

		swap(&arr[i], &arr[max]);
		i = max;
	}

}


void heapsort(int* arr, int size) {
	int i;
	for (i = size / 2 - 1 ; i >= 0; i--)
		heapify(arr, size, i);

	while (size > 0) {
		heapify(arr, size, 0);
		size--;
		swap(&arr[size], &arr[0]);

	}
	
}


int main()
{
	FILE *iFile;
	FILE *oFile;
	iFile = fopen("in.txt", "r");
	oFile = fopen("out.txt", "w");
	int size;
	fscanf(iFile, "%d", &size);
	int* arr = (int*)malloc(sizeof(int)*size);
	int i;
	for (i = 0; i < size; i++)
		fscanf(iFile, "%d", &arr[i]);

	heapsort(arr, size);
	
	for (i = 0; i < size; i++)
		fprintf(oFile, "%d ", arr[i]);

	
	fclose(iFile);
	fclose(oFile);
	free(arr);
	return 0;
	

}

