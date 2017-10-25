#include <stdio.h>
#include <stdlib.h>

void outputArray(FILE *output_file, int *array, int n) {
    for (int *arrayend = array + n; array < arrayend; ++array) fprintf(output_file, "%d ", *array);
    fclose(output_file);
    free(array);
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
    int *arrayend = array + *n;
    for (; array < arrayend; ++array) fscanf(input_file, "%d ", array);
    fclose(input_file);
    heapSort(array, n);
    return array;
}


int main() {
    int n;
    outputArray(fopen("out.txt", "w"), inputArray(fopen("in.txt", "r"), &n), n);
    return 0;
}
