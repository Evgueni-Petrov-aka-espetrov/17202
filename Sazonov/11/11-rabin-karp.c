#include <stdio.h>

#define MAX_SIZE 18

void h(char first_symb, int *hash, char new_symb, int max_power) {
    *hash = (*hash - (first_symb % 3) + 1) / 3 + ((new_symb % 3) * max_power);
}

int main() {
    FILE *input_file = fopen("in.txt", "r"), *output_file = fopen("out.txt", "w");
    int power[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907, 43046721};
    char input_buffer[MAX_SIZE];
    fgets(input_buffer, MAX_SIZE, input_file);
    int pattern_length = 0, pattern_hash = 0;
    for (; input_buffer[pattern_length] != '\n' && input_buffer[pattern_length] != '\0'; ++pattern_length)
        pattern_hash += input_buffer[pattern_length] % 3 * power[pattern_length];
    fprintf(output_file, "%d ", pattern_hash);
    int text_hash = 0, max_power = power[pattern_length - 1];
    for (int i = 0; i < pattern_length; ++i) {
        input_buffer[i] = (char) fgetc(input_file);
        text_hash += (input_buffer[i] % 3) * power[i];
    }
    for (int i = 1; !feof(input_file); ++i) {
        if (pattern_hash == text_hash) {
            for(int j = i; j < i + pattern_length; ++j)
                fprintf(output_file, "%d ", j);
        }
        char input_tmp = (char) fgetc(input_file);
        h(input_buffer[i % pattern_length], &text_hash, input_tmp, max_power);
        input_buffer[i % pattern_length] = input_tmp;
    }
    fclose(input_file);
    fclose(output_file);
    return 0;
}
