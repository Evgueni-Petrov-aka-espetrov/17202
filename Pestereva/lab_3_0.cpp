#include <stdio.h>
#include <stdlib.h>

void todown(int arr[], int i, int N);

void read(int N, int arr[]) {
	for (int i = N - 1; i > N / 2 - 1; i--) {
		scanf("%d", &(arr[i]));
	}
	for (int i = N / 2 - 1; i >= 0; i--) {
		scanf("%d", &(arr[i]));
		todown(arr, i, N);
	}
}

void swap(int str[], int i, int j) {
	int k = str[j];
	str[j] = str[i];
	str[i] = k;
}

void todown(int arr[], int i, int N) {
	int maxn = i;
	while (true) {
		if (2 * i + 1 < N && arr[2 * i + 1] > arr[i])
			maxn = 2 * i + 1;
		if (2 * i + 2 < N && arr[2 * i + 2] > arr[maxn])
			maxn = 2 * i + 2;
		if (maxn == i)
			break;
		swap(arr, maxn, i);
		i = maxn;
	}
}

void sort(int N, int arr[]) {
	int last = N - 1;
	while (last > 0) {
		swap(arr, 0, last);
		todown(arr, 0, last);
		last--;
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
	sort(N, data);
	print(N, data);
	free(data);
	return 0;
}