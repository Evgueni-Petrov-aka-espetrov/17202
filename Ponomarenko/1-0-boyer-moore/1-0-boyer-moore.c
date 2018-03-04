#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	const int lengthw = 16, lengths = 255;
	char word[lengthw], string[lengths];
	int j = 0, i = 0;
	char ch;
	do {
		ch = getchar();
		word[i] = ch;
		i++;
	} while (ch != '\n');
	word[i] = '\0';

	do {
		ch = getchar();
		string[j] = ch;
		j++;
	} while (ch != '\n');
	string[j] = '\0';
	
	
	int lengthword = i - 1, lengthstring = j - 1;
	 i = 0;
	 j = 0;
	int num_not_com = 0;
	int num_com = 0;
	
	
	
	while (i <= lengthstring - lengthword) {



		num_com = 0;
		num_not_com = 0;
		for (j = lengthword - 1; j >= 0; j--) {

			printf("%d ", i + j + 1);
			if (string[i + j] == word[j] && num_not_com == 0) {


				num_com++;
				if ((num_com == lengthword) && (num_not_com == 0)) {
					exit(0);
				}
			}
			else {
				int k;
				for (k = j; k >= 0; k--) {
					    if (string[i + j] == word[k]) {
							break;
						}
				}
				i = i +  j - k ;
				break;
			}
		}

	}

	return 0;
}
