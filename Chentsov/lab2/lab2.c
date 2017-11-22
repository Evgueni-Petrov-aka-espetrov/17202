#include <stdio.h>
#define MAX_PERMUTATION 10
int permutation_check(int permutation_length, const char* permutation){
    int i;
    for(i = 0;i < permutation_length;++ i)
        if(permutation[i] < permutation[i + 1])
            return 1;
    return 0;
}
int bad_input_check(int permutation_length, const char* permutation){
    int i, j, bad_input = 0;
    for(i = 0;i < permutation_length;++ i){
        if((permutation[i] < '0') || (permutation[i] > '9'))
            bad_input = 1;
        for(j = i + 1;j < permutation_length;++ j)
            if(permutation[i] == permutation[j])
                bad_input = 1;
    }
    if(bad_input){
        printf("bad input");
        return 0;
    }
    return 1;
}
int supporting_line_creating(char* supporting_line, const char* permutation, int permutation_length){
    supporting_line[0] = permutation[permutation_length - 1];
    int i = 1;
    while(permutation[permutation_length - 1 - i] > permutation[permutation_length - i]){
        supporting_line[i] = permutation[permutation_length - 1 - i];
        ++ i;
    }
    return i;
}
int searching(const char* supporting_line, const char* permutation, int permutation_length, int i){
    int j = 0;
    while(supporting_line[j] < permutation[permutation_length - 1 - i])
        j ++;
    return j;
}
void shifts(char* supporting_line, int j, int i, char* permutation, int permutation_length){
    char simbol = supporting_line[j];
    int k;
    for(k = j;k > 0;-- k)
        supporting_line[k] = supporting_line[k - 1];
    supporting_line[0] = simbol;
    for(k = i;k > j + 1;-- k)
        supporting_line[k] = supporting_line[k - 1];
    supporting_line[j + 1] = permutation[permutation_length - 1 - i];
    for(j = 0;j <= i;++ j)
        permutation[permutation_length - 1 - i + j] = supporting_line[j];
}
void permutations(int permutation_length, char* permutation, int N){
    char supporting_line[MAX_PERMUTATION];
    int i,j;
    while((permutation_check(permutation_length,permutation)) && (N > 0)){
        i = supporting_line_creating(supporting_line,permutation,permutation_length);
        j = searching(supporting_line,permutation,permutation_length,i);
        shifts(supporting_line,j,i,permutation,permutation_length);
        printf("%s\n",permutation);
        -- N;
    }
}
int main(){
    char permutation[MAX_PERMUTATION + 1];
    scanf("%s",&permutation);
    int N, permutation_length = strlen(permutation);
    scanf("%i",&N);
    if(bad_input_check(permutation_length,permutation))
        permutations(permutation_length,permutation,N);
    return 0;
}
