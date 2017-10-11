#include <stdio.h>
#include <string.h>

unsigned int hash(char buf[], int templ_len, unsigned int *max_pow) {
	int i = 0;
	*max_pow = 1;
	unsigned int s = 0;
	while (i < templ_len) {
		s += ((unsigned char)buf[i] % 3)**max_pow;
		i++;
		*max_pow *= 3;
	}
	*max_pow /= 3;
	return s;
}
unsigned int hash_shift(unsigned char buf_first, unsigned char buf_last, unsigned int prev_hash, unsigned int max_pow) {
	return (prev_hash - (buf_first % 3)) / 3 + (buf_last % 3)*max_pow;
}

int read(char buf[], int templ_len, int count, int *buf_pos, char *buf_first) {
	*buf_first = buf[*buf_pos];
	while (count--) {
		k = getchar();
		if (k == EOF) {
			return 0;
		}
		buf[*buf_pos] = (char)k;
		*buf_pos = (*buf_pos + 1) % templ_len;
	}
	return 1;
}

void search(char templ[], int templ_len) {
	char buf[17];
	int count = templ_len;
	int buf_pos = 0;
	char buf_first;
	int position = 1;
	unsigned int max_pow;
	unsigned int bhash;
	unsigned int thash = hash(templ, templ_len, &max_pow);
	printf("%d ", thash);
	while (read(buf, templ_len, count, &buf_pos, &buf_first)) {
		if (position != 1) {
			bhash = hash_shift(buf_first, buf[(buf_pos + templ_len - 1) % templ_len], bhash, max_pow);
		}
		else {
			bhash = hash(buf, templ_len, &max_pow);
		}
		if (bhash == thash) {
			for (int i = 0; i < templ_len; i++) {
				printf("%d ", position + i);
				if (buf[(buf_pos+i)%templ_len] != templ[i]) {
					break;
				}
			}
		}
		count = 1;
		position += count;
	}
}

int main() {
	char templ[17];
	gets_s(templ, 17);
	search(templ, strlen(templ));
	getchar();
}
