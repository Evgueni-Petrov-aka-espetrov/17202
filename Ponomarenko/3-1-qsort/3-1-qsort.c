include <stdio.h>
#include <stdlib.h>

void swap(int* a,int* b) {
	int help;
	help = *a;
	*a = *b;
	*b = help;
}

void sort(int* array, int num) {
	if ((num == 0) || (num == 1)) { return; };
	int i = 0, j = num - 1;
	int point = array[ rand() % num ];
	do {
		while (point > array[i]) {	i++;}
		while (point < array[j]) {  j--;}
		if (i <= j) {
			swap(&array[i], &array[j]);
			i++;
			j--;
		}
	} while (i <= j);
	sort(array, j + 1);
	sort(array + i, num - i);
	return;
}

int main() {
	int num;//getting number of array
	scanf_s("%d", &num);
	int *array = (int*)malloc(2000000 * sizeof(int));
	for (int i = 0; i < num; i++) {//getting array
		scanf_s("%d",&array[i]);

	}

	sort(array, num);//sorting array
	for (int i = 0; i < num; i++) {//output of sorted array
		printf("%d ",array[i]);
	}
	free(array);
	return 0;
}
