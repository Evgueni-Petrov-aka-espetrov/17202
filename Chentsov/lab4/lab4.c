#include <stdio.h.>
#include <string.h>
#define RADIX 10
#define MAX_STR 1000
int calculation_about_brackets(char symbol,int open_brackets_number,int *extra_brackets){
    if(symbol == '(')
        ++ open_brackets_number;
    if(symbol == ')')
        -- open_brackets_number;
    if(open_brackets_number == -1)
        *extra_brackets = 0;
    return open_brackets_number;
}
int test_about_brackets(const char *line, int length, int *extra_brackets){
    int i = 1, open_brackets_number = 0;
    if((line[0] == '(') && (line[length - 1] == ')')){
        *extra_brackets = 1;
        while(i < length - 1){
            open_brackets_number = calculation_about_brackets(line[i],open_brackets_number,extra_brackets);
            if(open_brackets_number < -1)
                return 1;
            ++ i;
        }
    }
    return 0;
}
int test_near_brackets(const char *line, int i, int length, int *open_brackets_number){
    if(line[i] == '('){
        if(((i < length - 1) && ((line[i + 1] != '(') && ((line[i + 1] < '1') || (line[i + 1] > '/' + RADIX)))) || ((i > 0) && (line[i - 1] != '(') && (line[i - 1] != '+') && (line[i - 1] != '-') && (line[i - 1] != '*') && (line[i - 1] != '/')))
            return 1;
        ++ *open_brackets_number;
    }
    if(line[i] == ')'){
        if(((i > 0) && ((line[i - 1] != ')') && ((line[i - 1] < '0') || (line[i - 1] > '/' + RADIX)))) || ((i < length - 1) && (line[i + 1] != ')') && (line[i + 1] != '+') && (line[i + 1] != '-') && (line[i + 1] != '*') && (line[i + 1] != '/')))
            return 1;
        -- *open_brackets_number;
    }
    return 0;
}
void operations_search(int open_brackets_number, int *last_addition, const char *line, int i, int *last_multiplication){
    if((open_brackets_number == 0) && (*last_addition == 0) && ((line[i] == '+') || (line[i] == '-')))
        *last_addition = i;
    if((open_brackets_number == 0) && (*last_multiplication == 0) && ((line[i] == '*') || (line[i] == '/')))
        *last_multiplication = i;
}
int errors_and_operations_search(const char *line, int length, int *last_addition, int *last_multiplication){
    int i = length - 1, open_brackets_number = 0;
    while((i >= 0) && (((line[i] >= '(') && (line[i] <= '+')) || (line[i] == '-') || ((line[i] >= '/') && (line[i] <= '/' + RADIX)))){
        if(test_near_brackets(line,i,length,&open_brackets_number))
            return 1;
        operations_search(open_brackets_number,last_addition,line,i,last_multiplication);
        -- i;
    }
    if((i > -1) || (open_brackets_number != 0))
        return 1;
    return 0;
}
int addition(const char *line, int last_addition, int *error, int length){
    if(line[last_addition] == '+')
        return line_calculation(line,last_addition,error) + line_calculation(line + last_addition + 1,length - last_addition - 1,error);
    return line_calculation(line,last_addition,error) - line_calculation(line + last_addition + 1,length - last_addition - 1,error);
}
int multiplication(const char *line, int last_multiplication, int *error, int length){
    if(line[last_multiplication] == '*')
        return line_calculation(line,last_multiplication,error) * line_calculation(line + last_multiplication + 1,length - last_multiplication - 1,error);
    int denominator = line_calculation(line + last_multiplication + 1,length - last_multiplication - 1,error);
    if(denominator)
        return line_calculation(line,last_multiplication,error) / denominator;
    *error = 2;
    return 0;
}
int number_calculation(const char *line, int length){
    int i, value = 0;
    for(i = 0;i < length;++ i){
        value *= RADIX;
        value += line[i] - '0';
    }
    return value;
}
int line_calculation(const char *line, int length, int *error){
    int extra_brackets = 0, last_addition = 0, last_multiplication = 0;
    if((test_about_brackets(line,length,&extra_brackets)) || (length == 0) || (line[0] == '+') || (line[0] == '-') || (line[0] == '*') || (line[0] == '/') || (line[length - 1] == '+') || (line[length - 1] == '-') || (line[length - 1] == '*') || (line[length - 1] == '/') || (errors_and_operations_search(line,length,&last_addition,&last_multiplication))){
        *error = 1;
        return 0;
    }
    if(extra_brackets)
        return line_calculation(line + 1,length - 2,error);
    if(last_addition > 0)
        return addition(line,last_addition,error,length);
    if(last_multiplication > 0)
        return multiplication(line,last_multiplication,error,length);
    return number_calculation(line,length);
}
int main(){
    char line[MAX_STR];
    gets(line);
    int value, error = 0;
    value = line_calculation(line,strlen(line),&error);
    if(error == 0)
        printf("%i",value);
    if(error == 1)
        printf("syntax error");
    if(error == 2)
        printf("division by zero");
    return 0;
}
