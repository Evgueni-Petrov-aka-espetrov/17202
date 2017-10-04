#include <stdio.h>
#include <math.h>

int checkB1B2(int b1, int b2) { // проверка на корректные значения b1 и b2
    if (b1 < 2 || b2 < 2 || b1 > 16 || b2 > 16) return 1;
    return 0;
}

int inputNcheck(FILE *input_file, char *input_array, int *dotnum, int *endnum) {
    // поразрядный ввод числа в СС b1 в массив, проверка на корректность строки
    int enumerator = -1;
    do {
        fscanf(input_file, "%c", &input_array[++enumerator]);
        if (input_array[enumerator] == '.') {
            if (*dotnum != -1) { // более одной точки в строке => badinput
                return 1;
            }
            *dotnum = enumerator;
        }
        if(input_array[enumerator] == '\r'){
            --enumerator;
        }
    } while (input_array[enumerator] != '\n'); // ввод всей строки
    *endnum = enumerator;
    if (*dotnum == *endnum - 1 || *dotnum == 0 || *endnum == 0)
        return 1; // точка в конце строки, в начале строки или пустая строка => badinput
    return 0;
}

int charToDecValue(char *input_array, int b1, int *dotnum, int *endnum) {
// перевод символа в численное значение, проверка на то, что каждая цифра удов. неравенству 0 <= x < b1
    for (int i = 0; i < *endnum; i++) {
        if (i == *dotnum) continue;
        input_array[i] -= '0';
        if (input_array[i] > 9) {
            input_array[i] += '0' - 'A';
            if (input_array[i] > 9) input_array[i] += 'A' - 'a';
            input_array[i] += 10;
        }
        if (input_array[i] >= b1 || input_array[i] < 0) return 1;
    }
    return 0;
}

double B1toDec(char input_char, int b1, double power) {
    return (input_char * pow(b1, power));
}

long long maxB2pow(long long decimal, int b2) {
    // нахождение максимальной десятичной степени b2, которая меньше заданного числа в десятичном виде
    double power = b2;
    while (decimal > power) power *= b2;
    return power / b2;
}

char B2toChar(int b2_value) { // конвертация численного значения в символ
    char out[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    return out[b2_value];
}

double remain(double numerator, long long int denominator){
    return numerator - (floor(numerator / denominator) * denominator);
}

int main() {
    FILE *input_file, *output_file;
    input_file = fopen("in.txt", "rt");
    output_file = fopen("out.txt", "wt");

    if(input_file == NULL) return 1;

    int b1, b2, endnum = -1, dotnum = -1; // переменные для хранения CC b1, CC b2, длины числа и целой части числа соответственно

    fscanf(input_file, "%d %d\n", &b1, &b2);
    char input_array[13];

    if (checkB1B2(b1, b2) || inputNcheck(input_file, &input_array[0], &dotnum, &endnum) || charToDecValue(&input_array[0], b1, &dotnum, &endnum)) {
        fprintf(output_file, "bad input");
        fclose(input_file);
        fclose(output_file);
        return 0;
    }

    fclose(input_file);
    double decimal = 0;
    for (int power = 0; power < dotnum; ++power)
        decimal += B1toDec(input_array[power], b1, dotnum - power - 1);
    long long int power = maxB2pow(decimal, b2);
    while (power >= 1) {
        fprintf(output_file, "%c", B2toChar((int)  floor(decimal / power)));
        decimal = remain(decimal, power);
        power /= b2;
    }
    if (dotnum == -1) {
        fclose(output_file);
        return 0;
    }
    fprintf(output_file, ".");
    double decimal_d = 0;
    for (int q = -1; q > dotnum - endnum; --q) {
        decimal_d += B1toDec(input_array[dotnum - q], b1, q);
    }
    for (int q = 0; q < 13; ++q) {
        decimal_d *= b2;
        fprintf(output_file, "%c", B2toChar((int) decimal_d));
        decimal_d -= (int) decimal_d;
    }
    fclose(output_file);
    return 0;
}
