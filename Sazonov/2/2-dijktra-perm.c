#include <stdio.h>
#include <stdlib.h>

void error(FILE *output_file) {
	fprintf(output_file, "bad input\n");
	fclose(output_file);
	exit(0);
}

char *input_string(FILE *input_file) {
	char *string = (char*) malloc(11 * sizeof(char));
	fgets(string, 11, input_file);
	return string;
}

void swap(char *first, char *second) {
	char tmp = *first;
	*first = *second;
	*second = tmp;
}

void sortArr(char *string, const int from, const int to) {
	for (int i = from; i < to; ++i)
		for (int j = from; j < to; ++j)
			if (string[i] < string[j]) {
				swap(&string[i], &string[j]);
				break;
			}
}

void dijkstraPerm(char *string, FILE *output_file, const int pos, const int n, const int length) {
	if (pos == 0 || n == 0) {
		return;
	}
	if (string[pos] > string[pos - 1]) {
		for (int i = length - 1; i > pos - 1; --i) {
			if (string[i] > string[pos - 1]) {
				swap(&string[pos - 1], &string[i]);
				break;
			}
		}
		sortArr(&string[0], pos, length);
		fputs(string, output_file);
		dijkstraPerm(&string[0], output_file, length - 1, n - 1, length);
	}
	else {
		dijkstraPerm(&string[0], output_file, pos - 1, n, length);
	}
}

int main() {
	FILE *input_file = fopen("in.txt", "r"), *output_file = fopen("out.txt", "w");
	char *string = input_string(input_file);
	int n;
	fscanf(input_file, "%d", &n);
	fclose(input_file);
	int length = 0;
	int check[10] = {};
	while (string[length] != '\n' && string[length] != '\0') {
		if (string[length] > '9' || string[length] < '0') error(output_file);
		if (check[string[length] - '0'] == 0)
			check[string[length++] - '0'] = 1;
		else
			error(output_file);
	}
	dijkstraPerm(&string[0], output_file, length - 1, n, length);
	fclose(output_file);
	return 0;
}
