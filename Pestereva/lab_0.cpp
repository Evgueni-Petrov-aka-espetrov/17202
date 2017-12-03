#include<stdio.h>
#include<string.h>

const unsigned long long N = 10000000000000;
const char * alph = "0123456789abcdef";
const char * alphup = "0123456789ABCDEF";
const int M_LEN = 52;

int ch_to_int(int b, char k){
	for (int i = 0; i < b; i ++) {
		if (alph[i] == k || alphup[i] == k) {
			return i;
		}
	}
	return -1;
}

void swap(int arr[], int i, int j) {
	int k = arr[j];
	arr[j] = arr[i];
	arr[i] = k;
}

unsigned long long main_to_10(int b1, int main[]) {
	unsigned long long result = 0;
	unsigned long long pow = 1;
	for (int i = 13; i >= 0; i--) {
		if (main[i] == -1)
			continue;
		result += main[i] * pow;
		pow*=b1;
	}
	return result;
}

void reverse(int arr[]) {
	int k = 0;
	for (int i = 13; i > 0; i--) {
		if (arr[i] == -1)
			continue;
		if (k < i) {
			swap(arr, k, i);
			k++;
		}
		else
			break;
	}
}

void main_from_10(int b2, unsigned long long dec_m, int main[]) {
	for (int i = 0; i < M_LEN; i++) {
		main[i] = -1;
	}
	if (dec_m == 0)
	{
		main[0] = 0;
		return;
	}
	int i = 0;
	while (dec_m > 0 && i <M_LEN) {
		main[i] = dec_m%b2;
		dec_m /= b2;
		i++;
	}
	reverse(main);
}

int int_to_ch(int k) {
	return alph[k];
}

int read(int* b1, int* b2, int main[], int fraction[]) {
	int *buf = main;
	int i = 0;
	scanf("%d%d", b1, b2);
	if (*b1 < 2 || *b1 >16 || *b2 < 2 || *b2>16) {
		return -1;
	}
	while (1==1) {
		int k = getchar();
		if ((char)k == '\n' || (char)k == '\r') {
			continue;
		}
		if (k == EOF ) {
			break;
		}
		if ((char)k == ' ')
			continue;
		if ((char)k == '.') {
			if (buf != fraction) {
				buf = fraction;
				i = 0;
				continue;
			}
			else return -1;
		}
		buf[i] = ch_to_int(*b1, k);
		if (buf[i] == -1) {
			return -1;
		}
		i++;
	}
	if (main[0] == -1)
		return -1;
	if (buf == fraction && fraction[0] == -1)
		return -1;
}

void print(int main[], int fraction[]){
	int i = 0;
	while (i<M_LEN && main[i] != -1){
		printf("%c", int_to_ch(main[i]));
		i++;
	}
	if (fraction[0] != -1) {
		printf(".");
		i = 0;
		while (i<M_LEN && fraction[i] != -1) {
			printf("%c", int_to_ch(fraction[i]));
			i++;
		}
	}
}

unsigned long long fraction_to_10(int b1, int fraction[]) {
	unsigned long long result = 0;
	unsigned long long pow = 1;
	for (int i = 0; i < M_LEN; i++) {
		if (fraction[i] == -1)
			break;
		result = fraction[i] + result*b1;
		pow *= b1;
	}
	long double res1 = (long double)result/pow;
	unsigned long long res = res1 * N;
	return res;
}

void fraction_from_10(int b2, unsigned long long dec_f, int fraction[]) {
	for (int i = 0; i < M_LEN; i++) {
		fraction[i] = -1;
	}
	/*if (dec_f == 0)
	{
		fraction[0] = 0;
		return;
	}*/
	int i = 0;
	unsigned long long pow = 1;
	while (dec_f > 0 && i<M_LEN) {
		pow *= b2;
		fraction[i] = (dec_f*pow)/N;
		dec_f -= fraction[i]*N/pow;
		i++;
	}
}

int main() {
	int b1, b2;
	int main[M_LEN];
	int fraction[M_LEN];
	for (int i = 0; i < M_LEN; i++) {
		main[i] = -1;
		fraction[i] = -1;
	}
	if (read(&b1, &b2, main, fraction) == -1) {
		printf("bad input");
		return 0;
	}
	unsigned long long dec_m = main_to_10(b1, main);
	main_from_10(b2, dec_m, main);
	unsigned long long dec_f = fraction_to_10(b1, fraction);
	fraction_from_10(b2, dec_f, fraction);
	print(main, fraction);
}