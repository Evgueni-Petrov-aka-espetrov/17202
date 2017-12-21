#include <stdio.h>
#include <string.h>
#include <math.h>


void test(int templ_h, int test_h, const unsigned char* templ, const unsigned char* line,int i,int length,int len_templ){
    if(templ_h == test_h){
        int j = 0;

        while(templ[j] == line[i + j]){
            printf(" %i", length + i + j + 1);
            ++j;
            if(j == len_templ)
                break;
        }
        if(j < len_templ)
            printf(" %i", length + j + 1);
        }
}
void search_l(int* len_line, int len_templ, int* test_h, unsigned char* line, int templ_h, const unsigned char* templ, int* length){
    int i;

    for(i = 1;i <= *len_line - len_templ; ++i){
        *test_h = *test_h / 3 + (line[len_templ + i - 1] % 3) * pow(3,len_templ - 1);
        test(templ_h,*test_h,templ,line,i,*length,len_templ);
    }

    for(i = 0;i < len_templ; ++i)
        line[i] = line[*len_line - len_templ + i];

    *length += *len_line - len_templ;
    *len_line = len_templ;
}
void search_t(int* len_line, int len_templ, unsigned char* line,int templ_h, const unsigned char* templ,FILE *in){
    int i;
    int line_h=0;
    if(*len_line >= len_templ){
        for(i = 0; i < len_templ; ++i)
                line_h += (line[i] % 3) * pow(3,i);
        int test_h = line_h;
        test(templ_h,test_h,templ,line,0,0,len_templ);

        int length = 0;
        search_l(len_line,len_templ,&test_h,line,templ_h,templ,&length);

        int j;
        while(!feof(in)){
            fgets(line + *len_line,256,in);
            *len_line = strlen(line);
            search_l(len_line,len_templ,&test_h,line,templ_h,templ,&length);
        }
    }
}
int main(){
    FILE *in = fopen("in.txt","r");

    unsigned char templ[18];
    unsigned char line[256];

    fgets(templ,18,in);

    int len_templ = strlen(templ) - 1;

    int i;
    int line_h=0;

    for(i = 0;i < len_templ;++ i)
        line_h += (templ[i] % 3) * pow(3,i);
    int templ_h = line_h;

    printf("%i",templ_h);

    fgets(line,256,in);

    int len_line = strlen(line);
    search_t(&len_line,len_templ,line,templ_h,templ,in);

    fclose(in);
    return 0;
}
