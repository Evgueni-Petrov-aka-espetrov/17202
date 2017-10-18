#include <stdio.h>
#include <stdlib.h>

void outputArray(FILE *output_file, int *array, int n) {
    int *arrayend = array + n;
    for (; array < arrayend; ++array) fprintf(output_file, "%d ", *array);
    fclose(output_file);
    free(array);
}

void swapElements(int *first, int *second) {
    int buffer = *first;
    *first = *second;
    *second = buffer;
}

int partition(int *array, int low_index, int high_index) {
    int pivot = array[high_index]; // pivot >= array[k], low_index <= k <= high_index
    int i = low_index - 1; // array[low_index - 1] <= array[k], low_index <= k <= high_index

    for (int j = low_index; j < high_index; ++j)
        if (array[j] <= pivot) swapElements(&array[++i], &array[j]);

    swapElements(&array[++i], &array[high_index]);
    return i;
}

void quickSort(int *array, int low_index, int high_index) {
    if (low_index < high_index) {
        int part = partition(array, low_index, high_index);
        quickSort(array, low_index, part - 1);
        quickSort(array, part + 1, high_index);
    }
}

int *inputArray(FILE *input_file, int *n) {
    fscanf(input_file, "%d", n);
    int *array = (int *) malloc(*n * sizeof(int));
    int *arrayend = array + *n;
    for (; array < arrayend; ++array) fscanf(input_file, "%d ", array);
    fclose(input_file);
    quickSort(array, 0, *n - 1);
    return array;
}

int main() {
    int n;
    outputArray(fopen("out.txt", "w"), inputArray(fopen("in.txt", "r"), &n), n);
    return 0;
}
