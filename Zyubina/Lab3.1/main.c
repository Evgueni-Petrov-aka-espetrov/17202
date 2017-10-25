#include <stdio.h>
#include <stdlib.h>

void swap(int *array, int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

int partition(int *array, int low, int high)
{
    int pivot = array[low];
    int i = low - 1;
    int j = high + 1;
    while (1)
    {
            do {
                i++;
            }
            while (array[i]<pivot);
            do {
                j--;
            }
            while (array[j]>pivot);
            if (i>=j)
                return j;
            swap(array, i, j);
    }

}

void quicksort(int *array, int low, int high)
{
    if (low<high)
    {
        int p = partition(array, low, high);
        quicksort(array, low, p);
        quicksort(array, p+1, high); 
    }
}



int main()
{
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    int n, i;
    fscanf(in, "%d\n", &n);
    int *arr = malloc(n*sizeof(int));
    for (i=0;i<n;i++)
        fscanf(in, "%d", &arr[i]);
    quicksort(arr, 0, n-1);
    for (i=0;i<n;i++)
        fprintf(out, "%d ", arr[i]);
    free(arr);
    fclose(in);
    fclose(out);
    return 0;
}
