/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
*/

/*
* File: main.c
* Author: Антон
*
* Created on 18 сентября 2017 г., 12:20
*/

#include <stdio.h>
#include <stdlib.h>

/*
*
*/
#define bool int
#define false 0
#define true 1
#define EPSILON 1e-3



char char_to_num[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15};
char num_to_char[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void error(FILE *out)
{
    fprintf(out, "Bad input");
    exit(0);
}
void output_long_long(FILE *out, int radix, long long num, int offset_decimal_point)
{
    char temp_buf[64];
    int pos = 0;
    while (num)
    {
        temp_buf[pos++] = num_to_char[num % radix];
        num /= radix;
    };
    if (offset_decimal_point==pos)
    {
        fprintf(out, "0.");
    }
    if (pos==0)
    {
        fputc('0', out);
        return;
    }
    for (pos--;pos>=0;pos--)
    {
        fputc(temp_buf[pos], out);
        if (pos==offset_decimal_point) fputc('.', out);
    }
    if (offset_decimal_point==0) fputc('0', out);
}

void output_long_double(FILE *out, int radix, long double num)
{
    int shift = 0;
    long long rounded;
    while (true)
    {
        rounded = (long long) num;
        if (num<=rounded+EPSILON) break;
        num *= radix;
        shift++;
    }
    output_long_long(radix, rounded, shift);
}

bool input_num(FILE *in, int radix, long long* res, int* shift)
{
    bool was_point = false;
    *shift = 0;
    int symb = 0;
    *res = 0;
    int count_read = 0;
    while ((symb = fgetc(in))!=EOF)
    {
        count_read++;
        if (symb=='\n') continue;
        if (symb=='.')
        {
            if (was_point) return false;
            if (count_read==1) return false;
            was_point = true;
            continue;
        };
        if ((symb>='a') && (symb<='f')) symb-=('a'-'A');
        if ((symb<'0') && (symb>'F')) return false;
        symb = char_to_num[symb-'0'];
        if ((symb==-1) || (symb>=radix)) return false;
        (*res) *= radix;
        (*res) += symb;
        if (was_point) (*shift)++;
    };
    if ((was_point == true) && ((*shift)==0)) return false;
    return true;
};
int main(int argc, char** argv) {
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    int b1, b2;
    fscanf(in, "%d %d\n", &b1, &b2);
    if ((b1<2) || (b1>16)) error();
    if ((b2<2) || (b2>16)) error();
    long long temp_num;
    int shift;
    if (!input_num(in, b1, &temp_num, &shift)) error();
    if (shift==0)
        output_long_long(out, b2, temp_num, -1);
    else
    {
        long double num = temp_num;
        for (;shift>0;shift--)
        num /= b1;
        output_long_double(out, b2, num);
    };
    fclose(in);
    fclose(out);
    return 0;

};
