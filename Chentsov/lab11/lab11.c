#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_PATTERN 16
#define MAX 256
int h(int line_length, const unsigned char* line){
    int i, line_h = 0;
    for(i = 0;i < line_length;++ i)
        line_h += (line[i] % 3) * pow(3,i);
    return line_h;
}
void check(int pattern_h, int test_h, const unsigned char* pattern, const unsigned char* line,int i,int addtional_length,int pattern_length){
    if(pattern_h == test_h){
        int j = 0;
        while(pattern[j] == line[i + j]){
            printf(" %i",addtional_length + i + j + 1);
            ++ j;
            if(j == pattern_length)
                break;
        }
        if(j < pattern_length)
            printf(" %i",addtional_length + j + 1);
    }
}
void search_in_line(int* line_length, int pattern_length, int* test_h, unsigned char* line, int pattern_h, const unsigned char* pattern, int* addtional_length){
    int i;
    for(i = 1;i <= *line_length - pattern_length;++ i){
        *test_h /= 3;
        *test_h += (line[pattern_length + i - 1] % 3) * pow(3,pattern_length - 1);
        check(pattern_h,*test_h,pattern,line,i,*addtional_length,pattern_length);
    }
    for(i = 0;i < pattern_length; ++ i)
        line[i] = line[*line_length - pattern_length + i];
    *addtional_length += *line_length - pattern_length;
    *line_length = pattern_length;
}
void search_in_text(int* line_length, int pattern_length, unsigned char* line,int pattern_h, const unsigned char* pattern,FILE *fin){
    if(*line_length >= pattern_length){
        int test_h = h(pattern_length,line);
        check(pattern_h,test_h,pattern,line,0,0,pattern_length);
        int addtional_length = 0;
        search_in_line(line_length,pattern_length,&test_h,line,pattern_h,pattern,&addtional_length);
        int j;
        while(!feof(fin)){
            fgets(line + *line_length,MAX,fin);
            *line_length = strlen(line);
            search_in_line(line_length,pattern_length,&test_h,line,pattern_h,pattern,&addtional_length);
        }
    }
}
int main(){
    unsigned char pattern[MAX_PATTERN + 2];
    FILE *fin;
    fin = fopen("in.txt","r");
    fgets(pattern,MAX_PATTERN + 2,fin);
    int pattern_length = strlen(pattern) - 1;
    pattern[pattern_length] = '\0';
    int pattern_h = h(pattern_length,pattern);
    printf("%i",pattern_h);
    unsigned char line[MAX];
    line[0] = '\0';
    fgets(line,MAX,fin);
    int line_length = strlen(line);
    search_in_text(&line_length,pattern_length,line,pattern_h,pattern,fin);
    fclose(fin);
    return 0;
}
