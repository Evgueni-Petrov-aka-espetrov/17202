#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int *input(int *N){
    scanf("%i",N);
    int i;
    int *sortable = malloc(sizeof(int) * *N);
    for(i = 0;i < *N;++ i)
        scanf("%i",&sortable[i]);
    return sortable;
}
void substitution(int *first, int *second){
    *first ^= *second;
    *second ^= *first;
    *first ^= *second;
}
void riddling(int start, int stop, int *sortable){
    int base = start, left, right;
    left = 2 * base + 1;
    right = left + 1;
    while(left < stop){
        if(right < stop)
            if(sortable[left] > sortable[right])
                if(sortable[base] < sortable[left]){
                    substitution(&sortable[base],&sortable[left]);
                    base = left;
                }
                else
                    break;
            else
                if(sortable[base] < sortable[right]){
                    substitution(&sortable[base],&sortable[right]);
                    base = right;
                }
                else
                    break;
        else{
            if(sortable[base] < sortable[left]){
                substitution(&sortable[base],&sortable[left]);
                base = left;
            }
            break;
        }
        left = 2 * base + 1;
        right = left + 1;
    }
}
int heap_creation(int N, int *sortable){
    int heap_n = 1;
    while(heap_n < N){
        heap_n *= 2;
        ++ heap_n;
    }
    int i;
    for(i = (heap_n - 3) / 2;i >= 0;-- i)
        riddling(i,N,sortable);
    return heap_n;
}
int main(){
    int N;
    int *sortable = input(&N);
    int i, heap_n = heap_creation(N,sortable);
    for(i = N - 1;i > 0;-- i){
        substitution(&sortable[0],&sortable[i]);
        riddling(0,i,sortable);
    }
    for(i = 0;i < N;++ i)
        printf("%i ",sortable[i]);
    return 0;
}
