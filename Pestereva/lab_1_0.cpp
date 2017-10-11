#include <stdio.h>
#include <string.h>

int max(int a, int b) {
	return a > b ? a : b;
}

void fill_stops(int stops[], int stops_len, char templ[], int templ_len) {
	for (int i = 0; i < 256; stops[i++] = 0);
	for (int i = 0; i < templ_len - 1; i++, stops[(unsigned char)templ[i]] = i+1);
}

void fill_sufs(int sufs[], char templ[], int templ_len) {
	for (int slen = 0; slen <= templ_len; slen++) {
		int shift = 0;
		while (shift < templ_len) {
			shift++;
			if (slen + shift >= templ_len) {
				if (strncmp(templ, &(templ[shift]), templ_len-shift) == 0)
					break;
			}
			else {
				if (templ[templ_len-shift-slen-1]!= templ[templ_len - slen-1] && strncmp(&(templ[templ_len - shift - slen]), &(templ[templ_len - slen]), slen) == 0)
					break;
			}
		}
		sufs[slen] = shift;
	}
}

int shift_pos(char buf[], int templ_len, int count) {
	for (int i = count; i < templ_len; i++) {
		buf[i - count] = buf[i];
	}
	for (int i = templ_len - count; i < templ_len; i++) {
		int k = getchar();
		if (k == EOF) {
			return 0;
		}
		buf[i] = k;
	}
	return 1;
}

void search(char templ[], int templ_len) {
	int stops[256];
	int sufs[17];
	fill_stops(stops, 256, templ, templ_len);
	fill_sufs(sufs, templ, templ_len);
	char buf[17];
	int shift = templ_len;
	int pos_buf = 1;
	while (shift_pos(buf, templ_len, shift)) {
		pos_buf += shift;
		shift = 0;
		for (int i = templ_len - 1; i >= 0; i--) {
			printf("%d ", pos_buf - templ_len + i);
			if (buf[i] != templ[i]) {
				shift = max(i - stops[buf[i]] + 1, sufs[templ_len - i - 1]);
				break;
			}
		}
		if (shift == 0) {
			shift = sufs[templ_len];
		}
	}
}

int main() {
	char templ[17];
	gets_s(templ, 17);
	search(templ, strlen(templ));
	getchar();
}
