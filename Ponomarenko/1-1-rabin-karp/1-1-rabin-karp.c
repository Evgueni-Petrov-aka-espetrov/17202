#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>


int hash(unsigned char c, int last_step) {//counting hash of one symbol
	static int degree = 1;
	int res = (c % 3) * degree;
	degree *= 3;
	if (last_step == 1) {
		degree = 1;
	}
	return  res;
}

int word_hash(unsigned char* word, int lengthword) {//counting hash of array of symbols
	int  sum = 0;
	for (int i = 0; i < lengthword; i++) {
		sum += hash(word[i], lengthword - i);
	}
	return sum;
}

bool check(unsigned char* word, unsigned char* string, const int lengthword, int from_position) {
	//comparing symbols
	for (int i = 0; i < lengthword; ++i) {
		if (word[i] == string[i]) {
			printf("%d ", from_position + i + 1);
		}
		else {
			//printf("Didn't concide ");
			printf("%d ", from_position + i + 1);

			return false;
		}
	}
	return true;
}

int pow(int a, int b)
{
	int result = 1;
	for (int i = 0; i < b; i++)
	{
		result *= a;
	}
	return result;
}

int main() {
	
	const int lengthw = 16;
	int lengths = 255;
	unsigned char word[lengthw + 1]; // string[lengths];
	int  i = 0;
	char ch;

	do {//reading template
		ch = getchar();

		word[i] = ch;
		i++;
	} while (ch != '\n');
	--i;
	word[i] = '\0';
	int lengthword = i;
	unsigned char string[lengthw + 1];
	int hash_of_word = word_hash(word, lengthword);


	for (int j = 0; j < lengthword; j++) {
		ch = getchar();
		if (ch == EOF) { printf("0\n"); return 0; }
		string[j] = ch;
		string[j + 1] = '\0';
	}
	printf("%d ", hash_of_word);

	int hash_of_string = word_hash(string, lengthword);
	int k = 0;

	int factor = pow(3, lengthword - 1);
	while (1) {
		if (hash_of_word == hash_of_string) {//if hashes coincided
			check(word, string, lengthword, k);

		}
		char c = getchar();
		if (c == EOF) { break; }
		hash_of_string = ((hash_of_string - string[0] % 3) / 3 + ((unsigned char)c % 3) * factor);
		for (int i = 0; i < lengthword - 1; i++) {
			string[i] = string[i + 1];
		}
		string[lengthword - 1] = c;

		k++;
	}

	return 0;
}
