#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
using namespace std;

void swap(int *a, int *b) {
	int buffer = *a;
	*a = *b;
	*b = buffer;
}

void readArray(ifstream *fin, int *massToRead, const int massSize) {
	for (int i = 0; i < massSize; ++i) {
		*fin >> massToRead[i];
	}
}

void writeResult(const int *massToWrite, const int massSize) {
	for (int i = 0; i < massSize; ++i) {
		cout << massToWrite[i];
		if (i == (massSize - 1)) {
			break;
		}
		cout << ' ';
	}
}

int max(const int *mass, const int n, const int i, const int j, const int k) {
	int m = i;
	if (j < n && mass[j] > mass[m]) {
		m = j;
	}
	if (k < n && mass[k] > mass[m]) {
		m = k;
	}
	return m;
}

void downHeap(int *mass, const int massSize, int * i) {
	while (true) {
		int j = max(&mass[0], massSize, *i, 2 * *i + 1, 2 * *i + 2);
		if (j == *i) {
			break;
		}
		int t = mass[*i];
		mass[*i] = mass[j];
		mass[j] = t;
		*i = j;
	}
}

void heapSort(int *mass, const int massSize) {
	for (int i = (massSize - 2) / 2; i >= 0; --i) {
		downHeap(&mass[0], massSize, &i);
	}
	for (int i = 0; i < massSize; ++i) {
		int tmp = mass[massSize - i - 1];
		mass[massSize - i - 1] = mass[0];
		mass[0] = tmp;
		int zero = 0;
		downHeap(&mass[0], massSize - i - 1, &zero);
	}
}

void main() {
	int massSize;
	ifstream fin("in.txt");
	fin >> massSize;
	if (massSize == 0) {
		return;
	}

	int *mass = (int*)malloc(sizeof(int)*massSize);

	readArray(&fin, &mass[0], massSize);
	heapSort(&mass[0], massSize);
	writeResult(&mass[0], massSize);

	fin.close();
	free(mass);
	//system("pause");
}
