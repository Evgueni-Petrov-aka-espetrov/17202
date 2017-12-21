#include <stdio.h>
#include <stdlib.h>
int *input(int *N){
    scanf("%i",N);
    int i;
    int *sortable = malloc(sizeof(int) * *N);
    for(i = 0;i < *N;++ i)
        scanf("%i",&sortable[i]);
    return sortable;
}
int minimum(int first, int second){
    if(first < second)
        return first;
    return second;
}
void substitution(int *first, int *second){
    *first ^= *second;
    *second ^= *first;
    *first ^= *second;
}
void sorting(int N, int *sortable, int piloted){
    int left = 0;
    int right = N - 1;
    while(left < right){
        while(sortable[left] <= piloted)
            ++ left;
        while(sortable[right] > piloted)
            -- right;
        if(left < right)
            substitution(&sortable[left],&sortable[right]);
    }
    preparation_and_sorting(left,sortable);
    preparation_and_sorting(N - 1 - right,sortable + right + 1);
}
void preparation_and_sorting(int N, int *sortable){
    if(N < 2)
        return;
    int i, piloted;
    if(sortable[0] == sortable[N - 1]){
        i = 1;
        while((i < N - 1) && (sortable[0] == sortable[i]))
            ++ i;
        if(i == N - 1)
            return;
        piloted = minimum(sortable[0],sortable[i]);
    }
    else
        piloted = minimum(sortable[0],sortable[N - 1]);
    sorting(N,sortable,piloted);
}
int main(){
    int N;
    int *sortable = input(&N);
    preparation_and_sorting(N,sortable);
    int i;
    for(i = 0;i < N;++ i)
        printf("%i ",sortable[i]);
    return 0;
}
