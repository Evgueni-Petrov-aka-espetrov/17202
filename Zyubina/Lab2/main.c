#include <stdio.h>
#include <string.h>
#define max_string 10

int check(const char string[], int length) {
	int i,j;
	char test[] = "1234567890";
	for (i = 0; i < length; i++) {
		if (strchr(test, string[i]) != 0) {
			for (j = i+1; j < length; j++) {
				if (string[i] == string[j]) {
					printf("bad input");
					return 0;
				}
			}
		}
		else {
			printf("bad input");
			return 0;
		}
	}
	return 1;
}


void swap_symbol(char string[], int i, int j) {
	char a;
	a = string[j];
	string[j] = string[i];
	string[i] = a;
}

int new_string(char string[], int lenght) {
	int i = lenght - 2;
	while (i >= 0 && string[i] > string[i+1])
		i--;
    if (i == -1)
        return 0;
	int j = lenght;
	while (j>=0 && string[j] < string[i])
		j--;
	swap_symbol(string, i, j);
	i++;
	j = lenght-1;
	while (i < j)
	{
		swap_symbol(string, i, j);
		i++;
		j--;
	}
	return 1;
}
void print(const char string[], int lenght, int N) {
	int i = 0;
	while (i<N && new_string(string, lenght) == 1) {
		printf("%s\n", string);
		i++;
	}
}
int main() {
	char string[max_string + 1];
	int N;
	gets(string);
	scanf("%d", &N);
	if (check(string, strlen(string)))
		print(string, strlen(string), N);
	return 0;
}
