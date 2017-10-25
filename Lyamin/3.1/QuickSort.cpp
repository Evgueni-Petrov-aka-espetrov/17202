#include <iostream>
#include <fstream>

using namespace std;

void swap(int *a, int *b) {
	int buffer = *a;
	*a = *b;
	*b = buffer;
}

int get_pivot(int low, int high) {
	return low;
}

int partition(int* a, int low, int high) {
	int i = low;
	int j = high;
	while (i <= j) {
		while (a[i] < a[get_pivot(low, high)]) {
			i += 1;
		}

		while (a[j] > a[get_pivot(low, high)]) {
			j -= 1;
		}

		if (i <= j) {
			swap(&a[i], &a[j]);
			i += 1;
			j -= 1;
		}
	}
	return i;
}

void QuickSort(int* a, int low, int high) {
	if (low < high) {
		int p = partition(a, low, high);
		QuickSort(a, low, p - 1);
		QuickSort(a, p, high);
	}
	return;
}

int main() {
	ifstream fin("in.txt");
	int N;
	fin >> N;
	if (N == 0) {
		return 0;
	}

	int* original = (int*)malloc(sizeof(int)*N);
	for (int i = 0; i < N; ++i) {
		fin >> original[i];
	}

	QuickSort(&original[0], 0, N - 1);

	for (int i = 0; i < N; ++i) {
		cout << original[i];
		if (i == (N - 1)) {
			break;
		}
		cout << ' ';
	}
	
	free(original);
	return 0;
}
