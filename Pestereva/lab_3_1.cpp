#include <stdio.h>
#include <stdlib.h>

void swap(int arr[], int i, int j) {
	int k = arr[j];
	arr[j] = arr[i];
	arr[i] = k;
}

void read(int N, int arr[]) {
	for (int i = 0; i < N; i++) {
		scanf("%d", &(arr[i]));
	}
}

int getp(int arr[], int L, int R) {
	return arr[(L + R) / 2];
}

int partition(int arr[], int L, int R) {
	int p = getp(arr, L, R);
	int i = L;
	int j = R;
	while (i <= j) {
		while (arr[i] < p) {
			i++;
		}
		while (p < arr[j]) {
			j--;
		}
		if (i <= j) {
			swap(arr, i, j);
			i++;
			j--;
		}
	}
	return i;
}

void qsort(int arr[], int L, int R) {
	if (L < R) {
		int p = partition(arr, L, R);
		qsort(arr, L, p - 1);
		qsort(arr, p, R);
	}
}

void print(int N, const int arr[]) {
	for (int i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}
}

int main() {
	int N;
	int *data;
	scanf("%d", &N);
	data = (int*)malloc(sizeof(int)*N);
	read(N, data);
	qsort(data, 0, N-1);
	print(N, data);
	free(data);
	return 0;
}