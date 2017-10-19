#include <stdio.h>
#include <string.h>

int check(const char str[], int len) {
	char alph[] = "0123456789";
	for (int i = 0; i < len; i++) {
		if (strchr(alph, str[i]) != 0) {
			for (int j = i+1; j < len; j++) {
				if (str[j] == str[i]) {
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

void swap(char str[], int i, int j) {
	char k = str[j];
	str[j] = str[i];
	str[i] = k;
}

int new_str(char str[], int len) {
	int i = len - 2;
	while (i >= 0 && str[i] > str[i + 1])
		i--;
	if (i == -1)
		return 0;
	int j = len;
	int k;
	while (str[j] < str[i])
		j--;
	swap(str, i, j);
	i++;
	j = len-1;
	while (i < j)
	{
		swap(str, i, j);
		i++;
		j--;
	}
	return 1;
}
void print_n_str(const char str[], int len, int N) {
	int i = 0;
	while (i<N && new_str(str, len)==1) {
		printf("%s\n", str);
		i++;
	}
}
int main() {
	char str[11];
	int N;
	gets_s(str, 11);
	scanf_s("%d", &N);
	if (check(str, strlen(str))==1)
		print_n_str(str, strlen(str), N);
	return 0;
}