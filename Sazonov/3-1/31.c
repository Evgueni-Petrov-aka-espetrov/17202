#include <stdio.h>
#include <stdlib.h>

void inputArray(FILE *input_file, int n, int *array){
    int *arrayend = array + n;
    for (int *i = array; i < arrayend; ++i) fscanf(input_file, "%d ", i);
}

void outputArray(FILE *output_file, int n, int *array){
    int *arrayend = array + n;
    for (int *i = array; i < arrayend; ++i) fprintf(output_file, "%d ", *i);
}

void swapElements(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *array, int low_index, int high_index){
    int pivot = array[high_index]; // pivot >= array[k], low_index <= k <= high_index
    int i = low_index - 1; // array[low_index - 1] <= array[k], low_index <= k <= high_index

    for (int j = low_index; j < high_index; ++j)
        if(array[j] <= pivot) swapElements(&array[++i], &array[j]);

    swapElements(&array[++i], &array[high_index]);
    return i;
}

void quickSort(int *array, int low_index, int high_index){
    if(low_index < high_index){
        int part = partition(array, low_index, high_index);
        quickSort(array, low_index, part - 1);
        quickSort(array, part + 1, high_index);
    }
}

int main(){
    FILE *input_file = fopen("in.txt", "r");

    if(input_file == NULL){
        printf("INPUT ERROR");
        return 0;
    }

    int n = 0;
    fscanf(input_file, "%d", &n);

    int* array = malloc(n * sizeof(int));

    if(array == NULL){
        printf("MALLOC ERROR");
        return 0;
    }

    inputArray(input_file, n, array);

    quickSort(array, 0, n - 1);
    outputArray(fopen("out.txt", "w"), n, array);
    free(array);

    return 0;
}
