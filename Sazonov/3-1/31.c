#include <stdio.h>
#include <stdlib.h>

void outputArray(FILE *output_file, int *array, int n) {
    for (int i = 0; i < n; ++i) fprintf(output_file, "%d ", array[i]);
    fclose(output_file);
}

void swapElements(int *first, int *second) {
    int buffer = *first;
    *first = *second;
    *second = buffer;
}

void heapify(int arr[], int n, int i) {
    int maxIndex = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[maxIndex])
        maxIndex = l;

    if (r < n && arr[r] > arr[maxIndex])
        maxIndex = r;

    if (maxIndex != i) {
        swapElements(&arr[i], &arr[maxIndex]);
        heapify(arr, n, maxIndex);
    }
}

void heapSort(int *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; --i) {
        swapElements(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int *inputArray(FILE *input_file, int *n) {
    fscanf(input_file, "%d", n);
    int *array = (int *) malloc(*n * sizeof(int));
    for (int i = 0; i < *n; ++i) fscanf(input_file, "%d ", &array[i]);
    fclose(input_file);
    return array;
}


int main() {
    int n, *a = inputArray(fopen("in.txt", "r"), &n);
    heapSort(a, n);
    outputArray(fopen("out.txt", "w"), a, n);
    free(a);
    return 0;
}
