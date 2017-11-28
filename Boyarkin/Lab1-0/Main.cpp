#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable : 4996)

#define BUF_SIZE 100000
#define TABLE_SIZE 256
#define TEMPL_BUF 32
#define TEXT_BUF 3000
#define THRESHOLD 2700
#define OFFSET 2400

int readstr(FILE *fin, char *str, int max_count, const char* delim);
void shift(char *str, int offset);

int main()
{
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	char *templ = (char*)malloc(TEMPL_BUF * sizeof(char));
	char *text = (char*)malloc(TEXT_BUF * sizeof(char));

	if (!in || !out || !templ || !text) goto END;

	int templ_len = readstr(in, templ, TEMPL_BUF, "\n");
	int text_len = readstr(in, text, TEXT_BUF, "");

	int table[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; ++i)
		table[i] = templ_len;

	for (int i = 0; i < templ_len - 1; ++i)
		table[templ[i] + 128] = templ_len - i - 1;

	int templ_iter, text_iter;
	int text_ptr = templ_len - 1;
	int offset = 1;

	while (text_ptr < text_len)
	{
		if (text_ptr > THRESHOLD) {
			shift(text, OFFSET);

			text_ptr -= OFFSET;
			text_len -= OFFSET;
			offset += OFFSET;

			text_len += readstr(in, text + text_len, TEXT_BUF - text_len, "");
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
	
END:
	if (templ) free(templ);
	if (text) free(text);

	if (in) fclose(in);
	if (out) fclose(out);
	return 0;
}

int readstr(FILE *fin, char *str, int max_count, const char* delim) {
	int count = 0;

	while (--max_count) {
		*str = getc(fin);

		if (*str == EOF) break;
		for (const char *ptr = delim; *ptr; ++ptr) {
			if (*str == *ptr) goto END;
		}

		++count;
		++str;
	}

END:
	*str = '\0';

	return count;
}

void shift(char *str, int offset) {
	char *ptr;

	for (ptr = str + offset; *ptr; ++str, ++ptr)
		*str = *ptr;

	*str = *ptr;
}
