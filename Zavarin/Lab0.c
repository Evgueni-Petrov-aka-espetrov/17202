#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


char int_to_string(int number){
    char help;
    if ((0 <= number) && (number <= 9)){
        help = '0' + number;
    }
    if ((10 <= number) && (number <= 16)){

        help = 'a' + (number - 10);
    }
    return help;
}
int string_to_int(char sym) {

    int num = 0;
    if (('0' <= sym) && (sym <= '9')) {
        num = sym - '0';
    }
    else {
        if (('a' <= sym) && (sym <= 'z')) {
            num = sym - 'a' + 10;
        }
        else {
            if (('A' <= sym) && (sym <= 'Z')) {
                num = sym - 'A' + 10;
            }
        }
    }
    return num;
}
double string_to_double(char* sym) {

    double ch = 0;
    unsigned n;
    char* k = strchr(sym, '.');
    if (k == NULL){
        for (unsigned i = 0; i < strlen(sym); i++){
            ch = ch + string_to_int(sym[i])*pow(10, strlen(sym) - i - 1);
        }
    }
    else{
        for (unsigned i = 0; i < strlen(sym); i++){ // ������� ����� �����
            if (sym[i] == '.'){
                n = i;
                break;
            }
        };
        for (unsigned i = 0; i < n; i++){  // ��������� � ������ �� �� �����

            ch = ch + string_to_int(sym[i])*pow(10, n - i - 1);
        };
        for (unsigned i = n + 1; i < strlen(sym); i++){ // -||- ����� �����
            int digit = string_to_int(sym[i]);
            double p = 1 / pow(10, i - n);
            ch = ch + digit*p;
        };
    }
    return ch;
}
char* from_decimal(double input, unsigned target_ns) {
    char* result = (char*)malloc(100);
    char* result1 = (char*)malloc(100);
    char* result2 = (char*)malloc(100);
    int i = 0;
    long long partofint = (long long)input;
    double partofdouble = input - (double)partofint;
    int Z;
    do{
        result[i++] = int_to_string(partofint % target_ns);
        partofint = (long long)(partofint / target_ns);
    } while (partofint);
    result[i] = '\0';

    int l = i;
    for (i = 0; i < (int)(l / 2); i++){
        int time = result[i];
        result[i] = result[l - i - 1];
        result[l - i - 1] = time;
    };
    if (partofdouble){
        i = 0;
        while ((partofdouble > 0) && (i <= 11)){
            partofdouble = partofdouble*target_ns;
            Z = (int)partofdouble;
            partofdouble = partofdouble - Z;
            result1[i++] = int_to_string(Z);
        }
        int con = l + i + 1;


        for (int j = 0; j < l; j++){
            result2[j] = result[j];
        };
        result2[l] = '.';
        for (int j = l + 1; j < con; j++){
            result2[j] = result1[j - l - 1];

        };
        result2[con] = '\0';
        return result2;
    }
    else{
        free(result1);
        free(result2);
        return result;
    }
}
double to_decimal(char* input, unsigned source_ns) {
    double ch = 0;
    unsigned n;
    char* k = strchr(input, '.');
    if (k == NULL){
        for (unsigned i = 0; i < strlen(input); i++){
            ch = ch + string_to_int(input[i])*pow(source_ns, strlen(input) - i - 1);
        }
    }
    else{
        for (unsigned i = 0; i < strlen(input); i++){ // ������� ����� �����
            if (input[i] == '.'){
                n = i;
                break;
            }
        };
        for (unsigned i = 0; i < n; i++){  // ��������� � ������ �� �� �����

            ch = ch + string_to_int(input[i])*pow(source_ns, n - i - 1);
        };
        for (unsigned i = n + 1; i < strlen(input); i++){ // -||- ����� �����
            int digit = string_to_int(input[i]);
            double p = 1 / pow(source_ns, i - n);
            ch = ch + digit*p;
        };
    }
    return ch;
}



int test(char* std, int input){
    char per = int_to_string(input);
    int num = 0;
    int numfordot = 0;
    int numhelp = 15;
    for (unsigned i = 0; i < strlen(std); i++){
        if (('0' <= std[i]) && (std[i] < per)) {
            num++;
        }
        if (std[i] == '.'){
            numhelp = i;
            numfordot++;
        }
    }

    if ((strlen(std) > 13) || (numfordot>1) || ((strlen(std) != num) && (numfordot == 0)) || ((strlen(std) != num + 1) && (numfordot == 1)) || ((std[numhelp] == '.') && (std[numhelp + 1] == '\0')) || (numhelp==0) ){

        return 1;
    }
    else{
        return 0;
    }
}

int main(){
    char* std = (char*)malloc(13 * sizeof(*std));
    char* res = malloc(13);
    int input, output;
    double zap=0;
    double help=0;
    int g=0;
    scanf("%d %d", &input, &output);
    scanf("%s", std);
    if ((input < 2) || (input>16) || (output > 16) || (output < 2)){
        printf("bad input");
        getchar();
        g = 1;
    }

    if ((g != 1)&&(test(std, input) == 1) ){
        printf("bad input");
        getchar();
    } else{
        if (g != 1){
            if (input == output){
                printf("%s", std);
                getchar();
            }
            else{
                if (input == 10){
                    zap = string_to_double(std);
                    res = from_decimal(zap, output);
                    printf("%s", res);
                    getchar();
                }
                else{
                    if (output == 10){
                        printf("%f", to_decimal(std, input));
                        getchar();
                    }
                    else {
                        help = to_decimal(std, input);
                        char* help2 = from_decimal(help, output);
                        printf("%s", help2);
                        getchar();
                    }
                }
            }
        }
    }
    free(std);
    free(res);
    return 0;
}
