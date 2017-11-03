#include <stdio.h>
#include <string.h>
#define MAX_PATTERN 16
#define MAX 256
int pattern_input(unsigned char *pattern, FILE *fin){
    fgets(pattern,MAX_PATTERN + 2,fin);
    int pattern_length = strlen(pattern) - 1;
    pattern[pattern_length] = '\0';
    return pattern_length;
}
void stop_simbols_table_creation(int *stop_simbols_table, int pattern_length, const unsigned char *pattern){
    int i;
    for(i = 0;i < MAX;++ i)
        stop_simbols_table[i] = -1;
    for(i = 0;i < pattern_length - 1;++ i)
        stop_simbols_table[pattern[i]] = i;
}
int shift(int pattern_length, int i, const unsigned char *pattern){
    int j, condition, k;
    for(j = 1;j <= pattern_length;++ j){
        condition = 1;
        for(k = pattern_length - 1;k >= pattern_length - i;-- k)
            if(k - j >= 0)
                if(pattern[k] != pattern[k - j])
                    condition = 0;
        if(pattern_length - i - j - 1 >= 0)
            if(pattern[pattern_length - i - 1] == pattern[pattern_length - i - j - 1])
                condition = 0;
        if(condition){
            return j;
        }
    }
    return 0;
}
void suffix_table_creation(int pattern_length, const unsigned char *pattern, int *suffix_table){
    int i;
    for(i = 1;i <= pattern_length;++ i){
        suffix_table[i - 1] = shift(pattern_length,i,pattern);
    }
}
int reading(unsigned char *text, int text_length, FILE *fin){
    fgets(text + text_length,MAX,fin);
    text_length = strlen(text);
    text[text_length] = '\0';
    return text_length;
}
void reading_and_searching(FILE *fin, int pattern_length, const unsigned char *pattern, const int *stop_simbols_table,const int *suffix_table){
    int text_length = 0, i, j = 0, additional_length = 0;
    unsigned char text[MAX];
    text[0] = '\0';
    while(!feof(fin)){
        text_length = reading(text,text_length,fin);
        text_length = reading(text,text_length,fin);
        i = pattern_length - 1;
        while((i + j < text_length) && (text_length > 0)){
            if(i >= 0)
                while(pattern[i] == text[i + j]){
                    printf("%i ",additional_length + i + j + 1);
                    -- i;
                }
            if(i == pattern_length - 1){
                printf("%i ",additional_length + i + j + 1);
                j += i - stop_simbols_table[text[i + j]];
            }
            else
                if(i > -1){
                    printf("%i ",additional_length + i + j + 1);
                    j += suffix_table[pattern_length - 1 - i];
                    i = pattern_length - 1;
                }
                else{
                    i = pattern_length - 1;
                    j += suffix_table[pattern_length - 1];
                }
        }
        for(i = 0;i < pattern_length;++ i)
            text[i] = text[text_length - pattern_length + i];
        additional_length += text_length - pattern_length;
        j -= text_length - pattern_length;
        text_length = pattern_length;
    }
}
int main(){
    FILE *fin;
    fin = fopen("in.txt","r");
    unsigned char pattern[MAX_PATTERN];
    int pattern_length = pattern_input(pattern,fin);
    int stop_simbols_table[MAX];
    stop_simbols_table_creation(stop_simbols_table,pattern_length,pattern);
    int suffix_table[MAX_PATTERN];
    suffix_table_creation(pattern_length,pattern,suffix_table);
    reading_and_searching(fin,pattern_length,pattern,stop_simbols_table,suffix_table);
    fclose(fin);
    return 0;
}

