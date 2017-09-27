#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

void swap(int *arr, int x, int y)
{
    int temp = arr[x];
    arr[x] = arr[y];
    arr[y] = temp;
}
int partition(int *arr, int left, int right)
{
    /*int pivot = arr[right];
    int i = left - 1;
    int j;
    for (j = left; j<right; j++)
        if (arr[j] <= pivot)
        {
            i++;
            swap(arr, i,j);
        }
    swap(arr, i+1, right);
    return i + 1;*/
    int pivot = arr[left];
    int i = left - 1;
    int j = right + 1;
    while (true)
    {
            while (arr[++i]<pivot);
            while (arr[--j]>pivot);
            if (i>=j) return j;
            swap(arr, i, j);
    }
}
void quicksort(int *arr, int left, int right)
{
    if (left<right)
    {
        int p = partition(arr, left, right);
        quicksort(arr, left, p);
        quicksort(arr, p+1, right);
    }
}
int main()
{
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    int n, i;
    fscanf(in, "%d\n", &n);
    int *buf = malloc(n*sizeof(int));
    for (i=0;i<n;i++)
        fscanf(in, "%d", &buf[i]);
    quicksort(buf, 0, n-1);
    for (i=0;i<n;i++)
        fprintf(out, "%d ", buf[i]);
    free(buf);
    fclose(in);
    fclose(out);
    return 0;
}
