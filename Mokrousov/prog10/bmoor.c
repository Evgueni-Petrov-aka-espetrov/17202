#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAB_SIZE 256
#define TEMPLATE_BUF 32
#define TXT_BUF 900000
#define OFF (TXT_BUF-2*DELTA)
#define DELTA 300
#define THRES (TXT_BUF-DELTA)

int scan_string(FILE *fin, char *str, int max_count, const char* delim) {
	int count = 0;

	while (--max_count) {
		int temp = fgetc(fin);

		if (temp == EOF) break;
		*str = (char)temp;
		for (const char *ptr = delim; *ptr; ++ptr) {
			if (*str == *ptr) goto break_2;
		}

		++count;
		++str;
	}

break_2:
	*str = '\0';

	return count;
}


void shift_str(char *str, int offset) {
	char *ptr;
	for (ptr = str + offset; *ptr; ++str, ++ptr)
		*str = *ptr;
	*str = *ptr;
}

int main()
{
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");


	char *templ = (char*)malloc(TEMPLATE_BUF * sizeof(char));
	char *text = (char*)malloc(TXT_BUF * sizeof(char));

	int templ_len = scan_string(in, templ, TEMPLATE_BUF, "\n");
	int text_len = fread(text, sizeof(char), TXT_BUF, in);


	int table[TAB_SIZE];
	for (int i = 0; i < TAB_SIZE; ++i)
		table[i] = templ_len;


	for (int i = 0; i < templ_len - 1; ++i)
		table[(templ[i] + 128) % TAB_SIZE] = templ_len - i - 1;

	int templ_iter, text_iter;
	int text_ptr = templ_len - 1;
	int offset = 1;

	while (text_ptr < text_len)
	{
		if (text_ptr > THRES) {
			shift_str(text, OFF);

			text_ptr -= OFF;
			text_len -= OFF;
			offset += OFF;

			text_len += fread(text + text_len, sizeof(char), TXT_BUF - text_len, in);
		}

		templ_iter = templ_len - 1;
		text_iter = text_ptr;

		while (templ_iter >= 0)
		{
			fprintf(out, "%d ", text_iter + offset);

			if (templ[templ_iter] != text[text_iter]) break;

			--text_iter;
			--templ_iter;
		}
		text_ptr += table[text[text_ptr] + 128];
	}

	free(templ);
	free(text);

	fclose(in);
	fclose(out);
	return 0;
}


