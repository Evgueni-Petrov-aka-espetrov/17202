#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

void swap(int *a, int *b) {
	int buffer = *a;
	*a = *b;
	*b = buffer;
}

int pivot(const int first, const int last) {
	return first;
}

int partition(int* massToSort, const int first, const int last) {
	int i = first;
	int j = last;
	while (i <= j) {
		while (massToSort[i] < massToSort[pivot(first, last)])
		{
			++i;
		}
		while (massToSort[j] > massToSort[pivot(first, last)])
		{
			--j;
		}
		if (i <= j) {
			swap(&massToSort[i], &massToSort[j]);
			++i;
			--j;
		}
	}
	return i;
}

void quickSort(int *massToSort, const int first, const int last) {
	if (first < last) {
		int part = partition(massToSort, first, last);
		quickSort(massToSort, first, part - 1);
		quickSort(massToSort, part, last);
	}
	return;
}

void readArray(int *massToRead, const int massSize) {
	for (int i = 0; i < massSize; ++i) {
		cin >> massToRead[i];
	}
}

void writeResult(const int *massToWrite,const int massSize) {
	for (int i = 0; i < massSize; ++i) {
		cout << massToWrite[i];
		if (i == (massSize - 1)) {
			break;
		}
		cout << ' ';
	}
}

void main() {
	int massSize;
	cin >> massSize;

	if (massSize == 0) {
		return;
	}

	int *mass = (int*)malloc(sizeof(int)*massSize);

	readArray(&mass[0], massSize);
	quickSort(&mass[0], 0, massSize - 1);
	writeResult(&mass[0], massSize);

	free(mass);
}
