#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int* b) {
	int help;
	help = *a;
	*a = *b;
	*b = help;
}

void swifting(int* arr,int i, int num ) {
	if (i > (num /2 - 1)) {
		return;
	}
	if (arr[i] < arr[2 * i + 1] || (i * 2 + 2 < num && arr[i] < arr[2 * i + 2])) {
		if (arr[2 * i + 1]  > arr[2 * i + 2] || i * 2 + 2 >= num) {
			swap(&arr[i] , &arr[2 * i + 1]);
			swifting(arr, 2 * i + 1, num);
		}
		else {

			swap(&arr[i] , &arr[2 * i + 2]);
			swifting(arr, 2 * i + 2, num);
		}
	}
}

int main() {
	int num ;
	scanf("%d", &num);
	int *arr =(int*) malloc (num *sizeof(int));
	for (int i = 0; i < num; i++) {
		scanf("%d", &arr[i]);	
	}
	for (int i = num / 2 - 1; i >= 0; i--) {
		swifting(arr, i, num);
	}
	for (int i = num - 1; i >= 0; i--) {
		swap(&arr[i], &arr[0]);
		swifting(arr, 0, i);
		
	}
	for (int i = 0; i < num; i++) {
		printf("%d ",arr[i]);
	}
	free(arr);
	return 0;
}
