#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable : 4996)

#define BUF_SIZE 100000
#define TABLE_SIZE 256
#define TEMPL_BUF 16
#define TEXT_BUF 3000
#define THRESHOLD 2700
#define OFFSET 2400

int pow3[TEMPL_BUF];

void fill_pow3();
int read_pattern(FILE *in, unsigned char *buf);
long long hash_char(unsigned char ch, int i);
long long hash_str(int L, int R, const unsigned char *str);
bool cmp_char(FILE *out, int pos_buf, int pos_pattern, int pat_len, const unsigned char *buf, const unsigned char *pattern);
bool cmp_str(FILE *out, int from, int pat_len, const unsigned char *buf, const unsigned char *templ);

int main()
{
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	unsigned char *templ = (unsigned char*)malloc((TEMPL_BUF + 1) * sizeof(unsigned char));
	unsigned char *buf = (unsigned char*)malloc(TEXT_BUF * sizeof(unsigned char));

	char tmp_ch;
	int templ_len, pos, iter;
	long long templ_hash, hash;

	fill_pow3();
	templ_len = read_pattern(in, templ);

	pos = 0;
	while (pos < templ_len) {
		tmp_ch = fgetc(in);
		if (tmp_ch == EOF) { break; }

		buf[pos] = tmp_ch;
		++pos;
	}

	templ_hash = hash_str(0, templ_len, templ);
	fprintf(out, "%ld", templ_hash);

	if (pos < templ_len) { goto END; }
	hash = hash_str(0, templ_len, buf);

	if (hash == templ_hash) {
		cmp_str(out, 0, templ_len, buf, templ);
	}
	iter = 0;

	for (pos = templ_len; (tmp_ch = fgetc(in)) != EOF; ++pos) {
		hash -= buf[iter] % 3;
		hash /= 3;

		buf[iter] = tmp_ch;
		hash += (buf[iter] % 3) * pow3[templ_len - 1];
		if (hash == templ_hash) {
			cmp_str(out, pos - templ_len + 1, templ_len, buf, templ);
		}

		iter = (iter + 1) % templ_len;
	}

END:
	fclose(in);
	fclose(out);
	return 0;
}

void fill_pow3() {
	pow3[0] = 1;
	for (int i = 1; i < TEMPL_BUF; ++i) {
		pow3[i] = pow3[i - 1] * 3;
	}
}

int read_pattern(FILE *in, unsigned char *buf) {
	int len = 0;
	int ch;

	while (true) {
		ch = fgetc(in);
		if ((ch == EOF) || (ch == '\n')) { break; }

		*buf = ch;
		++buf;
		++len;
	}

	return len;
}

long long hash_char(unsigned char ch, int i) {
	return (ch % 3) * pow3[i];
}

long long hash_str(int L, int R, const unsigned char *str) {
	long long hash = 0;

	for (int i = L; i < R; ++i) {
		hash += hash_char(str[i], i);
	}
	
	return hash;
}

bool cmp_char(FILE *out, int pos_buf, int pos_pattern, int pat_len, const unsigned char *buf, const unsigned char *pattern)
{
	fprintf(out, " %d", pos_buf + 1);
	return (buf[pos_buf % pat_len] == pattern[pos_pattern]);
}

bool cmp_str(FILE *out, int from, int pat_len, const unsigned char *buf, const unsigned char *templ)
{
	for (int i = 0; i < pat_len; i++) {
		if (!cmp_char(out, from + i, i, pat_len, buf, templ)) return false;
	}
	return true;
}
