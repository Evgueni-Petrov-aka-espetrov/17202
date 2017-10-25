#include <stdio.h>
#include <string.h>
#define MAX_TEMPLATE 16
#define MAX 256
int template_input(unsigned char *template, FILE *fin){
    fgets(template,MAX_TEMPLATE + 2,fin);
    int template_length = strlen(template) - 1;
    template[template_length] = '\0';
    return template_length;
}
void stop_simbols_table_creation(int *stop_simbols_table, int template_length, const unsigned char *template){
    int i;
    for(i = 0;i < MAX;++ i)
        stop_simbols_table[i] = -1;
    for(i = 0;i < template_length - 1;++ i)
        stop_simbols_table[template[i]] = i;
}
int shift(int template_length, int i, const unsigned char *template){
    int j, condition, shift, k;
    for(j = 1;j <= template_length;j ++){
        condition = 1;
        shift = 0;
        for(k = template_length - 1;k >= template_length - i;k --)
            if(k - j >= 0)
                if(template[k] != template[k - j])
                    condition = 0;
        if(template_length - i - j - 1 >= 0)
            if(template[template_length - i - 1] == template[template_length - i - j - 1])
                condition = 0;
        if(condition){
            shift = j;
            return shift;
        }
    }
}
void suffix_table_creation(int template_length, const unsigned char *template, int *suffix_table){
    int i;
    for(i = 1;i <= template_length;i ++){
        suffix_table[i - 1] = shift(template_length,i,template);
    }
}
int reading(unsigned char *text, int text_length, FILE *fin){
    fgets(text + text_length,MAX,fin);
    return strlen(text);
}
void reading_and_searching(FILE *fin, unsigned char *text, int template_length, const unsigned char *template, const int *stop_simbols_table,const int *suffix_table){
    int text_length = 0, i, j = 0, additional_length = 0;
    while(!feof(fin)){
        text_length = reading(text,text_length,fin);
        text_length = reading(text,text_length,fin);
        i = template_length - 1;
        while((i + j < text_length) && (text_length > 2)){
            if(i >= 0)
                while(template[i] == text[i + j]){
                    printf("%i ",additional_length + i + j + 1);
                    i --;
                }
            if(i == template_length - 1){
                printf("%i ",additional_length + i + j + 1);
                j += i - stop_simbols_table[text[i + j]];
            }
            else
                if(i > -1){
                    printf("%i ",additional_length + i + j + 1);
                    j += suffix_table[template_length - 1 - i];
                    i = template_length - 1;
                }
                else{
                    i = template_length - 1;
                    j += suffix_table[template_length - 1];
                }
        }
        for(i = 0;i < template_length;++ i)
            text[i] = text[text_length - template_length + i];
        additional_length += text_length - template_length;
        j -= text_length - template_length;
        text_length = template_length;
    }
}
int main(){
    FILE *fin;
    fin = fopen("in.txt","r");
    unsigned char template[MAX_TEMPLATE + 1], text[MAX];
    int template_length = template_input(template,fin);
    int stop_simbols_table[MAX];
    stop_simbols_table_creation(stop_simbols_table,template_length,template);
    int suffix_table[MAX_TEMPLATE];
    suffix_table_creation(template_length,template,suffix_table);
    reading_and_searching(fin,text,template_length,template,stop_simbols_table,suffix_table);
    fclose(fin);
    return 0;
}
