#include <stdio.h>
#include <stdlib.h>

void sift_down(int len, int *data, int pos)
{
    int mn = pos;
    int a = data[pos];
    while ((pos << 1) <= len)
    {
        mn = pos << 1;
        if (((mn|1)<=len) && (data[mn|1]<data[mn])) mn |= 1;
        if (data[mn]>a) break;
        data[pos] = data[mn];
        pos = mn;
    }
    data[pos] = a;
}
void build_heap(int len, int *data)
{
    int i;
    for (i=len>>1;i>0;i--)
        sift_down(len, data, i);
}
void swap(int *data, int x, int y)
{
    int temp = data[x];
    data[x] = data[y];
    data[y] = temp;
}
int main()
{
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    int arr_len, i;
    fscanf(in, "%d\n", &arr_len);
    int *data_arr = malloc(arr_len*sizeof(int));
    for (i=0;i<arr_len;i++)
        fscanf(in, "%d", &data_arr[i]);
    data_arr--;
    build_heap(arr_len, data_arr);
    i = arr_len;
    while(i)
    {
        swap(data_arr, 1, i--);
        sift_down(i, data_arr, 1);
    }
    while (arr_len)
        fprintf(out, "%d ", data_arr[arr_len--]);
    free(data_arr+1);
    fclose(in);
    fclose(out);
    return 0;
}
