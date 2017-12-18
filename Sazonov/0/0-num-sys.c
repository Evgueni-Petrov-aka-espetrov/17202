#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void error(FILE *output_file) {
    fclose(output_file);
    output_file = fopen("out.txt", "w");
    fprintf(output_file, "bad input");
    fclose(output_file);
    exit(0);
}

char int_symbol(int in) {
    char out[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    return out[in];
}


void integral_part(long long in, int base, FILE *output_file) {
    if (in < base) {
        fputc(int_symbol((int) in), output_file);
        return;
    }
    integral_part(in / base, base, output_file);
    fputc(int_symbol((int) (in % base)), output_file);
}

void fractional_part(double input, int b2, FILE *output_file) {
    for (int i = 0; i < 13; ++i) {
        fputc(int_symbol((int) (input * b2)), output_file);
        input = input * b2 - (int) (input * b2);
    }
}

char char_to_value(char ch, int b1, FILE *output_file) {
    if (ch >= '0' && ch <= '9') ch = (char) (ch - '0');
    if (ch >= 'a' && ch <= 'f') ch = (char) (ch + 10 - 'a');
    if (ch >= 'A' && ch <= 'F') ch = (char) (ch + 10 - 'A');
    if (ch >= b1) error(output_file);
    return ch;
}

void fractional_char(char *string, FILE *output_file, int b1, int b2) {
    double result = (double) char_to_value(string[0], b1, output_file) / b1;
    int powers[14] = {b1};
    for (int i = 1; string[i] != '\n' && string[i] != '\0'; ++i) {
        powers[i] = powers[i - 1] * b1;
        result += (double) char_to_value(string[i], b1, output_file) / powers[i];
        if(result > 0.99999990000000000)
            break;
    }
    fractional_part(result, b2, output_file);
}

int main() {
    FILE *input_file = fopen("in.txt", "r"), *output_file = fopen("out.txt", "w");
    int b[2];
    char string[14];
    fscanf(input_file, "%d %d\n", &b[0], &b[1]);
    fgets(string, 14, input_file);
    fclose(input_file);
    if (b[0] < 2 || b[0] > 16 || b[1] < 2 || b[1] > 16) error(output_file);
    char *endptr;
    integral_part(strtoll(string, &endptr, b[0]), b[1], output_file);
    if ((*endptr != '.' && *endptr != '\n') || endptr == &string[0]) error(output_file);
    if (*endptr == '\n') return 0;
    if (*endptr == '.' && *(endptr + 1) != '\n') {
        fputc('.', output_file);
        fractional_char(++endptr, output_file, b[0], b[1]);
    } else error(output_file);
    return 0;
}
