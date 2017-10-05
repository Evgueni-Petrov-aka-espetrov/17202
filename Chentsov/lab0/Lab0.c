#include <stdio.h>
#include <string.h>
#include <math.h>
void lower(int length, char *x){
    int i;
    for(i = 0;i < length;i ++)
        if((x[i] >= 'A') && (x[i] <= 'F'))
            x[i] += 'a' - 'A';
}
int hex2int(int hex){
    if(hex <= '9')
        return hex - '0';
    return hex - 'a' + 10;
}
int int2hex(int in){
    if(in < 10)
        return in + '0';
    return in + 'a' - 10;
}
int check_the_number(int length, const char *x, int b1, int b2, int point){
    int i, bad_input = 0;
    for(i = 0;i < length;i ++)
        if(x[i] == '.')
            bad_input ++;
        else
            if(b1 < 11){
                if((x[i] < '0') || (x[i] > '0' + b1 - 1))
                    bad_input += 2;
            }
            else
                if(((x[i] < '0') || (x[i] > '9')) && ((x[i] < int2hex(10)) || (x[i] > int2hex(15))))
                    bad_input += 2;
    if((b1 < 2) || (b1 > 16) || (b2 < 2) || (b2 > 16) || (bad_input > 1) || (x[length - 1] == '.') || (point == 0)){
        printf("bad input");
        return 0;
    }
    return 1;
}
long long calculation_of_integer_part(int point, const char *x, int b1){
    int i;
    long long result = 0;
    for(i = 0;i < point;i ++)
        result += hex2int(x[i]) * (long long)powl(b1,(point - i - 1));
    return result;
}
double calculation_of_fractional_part(int point, int length, const char *x, int b1){
    int i;
    double result = 0;
    for(i = point + 1;i < length;i ++)
        result += hex2int(x[i]) * (float)powl(b1,(point - i));
    return result;
}
int calculation_of_length_of_integer_part(int b2, long long integer_part){
    int result = 1;
    while(pow(b2,result) <= integer_part)
        result ++;
    return result;
}
void making_of_integer_part(int length_of_integer_part, int b2, char *line, long long integer_part){
    long long powered_b2;
    int i;
    for(i = 0;i < length_of_integer_part;i ++){
        powered_b2 = powl(b2,length_of_integer_part - i - 1);
        line[i] = int2hex(integer_part / powered_b2);
        integer_part -= (integer_part / powered_b2) * powered_b2;
    }
}
int making_of_fractional_part(double fractional_part, char *line, int length_of_integer_part, int b2){
    int i = 0;
    if(fractional_part > 0){
        line[length_of_integer_part] = '.';
        i = 1;
        int remainder;
        while((fractional_part != 0) && (i < 13)){
            fractional_part *= b2;
            remainder = (int)fractional_part % b2;
            line[length_of_integer_part + i] = int2hex(remainder);
            fractional_part -= remainder;
            i ++;
        }
    }
    return i;
}
int main() {
    int b1,b2;
    scanf("%i %i",&b1,&b2);
    char x[14];
    scanf("%s",&x);
    int length = strlen(x);
    int i;
    lower(length,x);
    int point = strchr(x,'.') - x;
    int no_point = 1;
    if(point < 0)
        point = length;
    if(check_the_number(length, x, b1, b2, point)){
        long long integer_part = calculation_of_integer_part(point, x, b1);
        double fractional_part = calculation_of_fractional_part(point, length, x, b1);
        int length_of_integer_part = calculation_of_length_of_integer_part(b2, integer_part);
        char line[length_of_integer_part + 13];
        making_of_integer_part(length_of_integer_part, b2, line, integer_part);
        i = making_of_fractional_part(fractional_part, line, length_of_integer_part, b2);
        length = strlen(line);
        line[length_of_integer_part + i] = '\0';
        printf("%s",line);
    }
    return 0;
}
