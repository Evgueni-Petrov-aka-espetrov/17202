#include <cctype>
#include <stdio.h>
#include <string.h>
#pragma warning (disable : 4996)

#define MAX_LEN 10

int transpos_check(const char *t);
int transpos_next(char *t, int len);
void reverse(char *left, char *right);
void swap(char *lhs, char *rhs);

int main()
{
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	if (!in || !out) goto END;

	char str[MAX_LEN + 1];
	int len;

	fscanf(in,"%10s", str);
	len = strlen(str);

	if (transpos_check(str)) {
		int count;
		fscanf(in, "%d", &count);

		while (count > 0) {
			if (!transpos_next(str, len)) break;

			fprintf(out, str);
			fprintf(out, "\n");

			--count;
		}
	}
	else fprintf(out, "bad input\n");

END:
	if (in) fclose(in);
	if (out) fclose(out);
	return 0;
}

int transpos_check(const char *t) {
	if (strlen(t) == 0) return 0;

	unsigned int flags = 0;
	int digit;

	for (; *t; ++t) {
		digit = *t - '0';
		if (!isdigit(*t)) return 0;

		if (flags & (1u << digit)) return 0;
		else flags |= 1u << digit;
	}

	return 1;
}

int transpos_next(char *t, int len) {
	char *end = t + len - 1;
	char *ptr = end - 1;

	while (ptr >= t) {
		if (*ptr < *(ptr + 1)) {
			
			while (*end < *ptr) --end;
			swap(ptr, end);
			reverse(ptr + 1, t + len - 1);

			return 1;
		}
		--ptr;
	}

	return 0;
}

void reverse(char *left, char *right) {
	while (left < right) {
		swap(left, right);

		++left;
		--right;
	}
}

void swap(char *lhs, char *rhs) {
	char temp = *lhs;
	*lhs = *rhs;
	*rhs = temp;
}
