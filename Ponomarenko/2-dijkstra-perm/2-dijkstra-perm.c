include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
	int help;
	help = *a;
	*a = *b;
	*b = help;
}

void sort(int* array, int num) {
	if ((num == 0) || (num == 1)) { return; };

	int i = 0, j = num - 1;
	int point = array[rand() % num];
	do {
		while (point > array[i]) { i++; }
		while (point < array[j]) { j--; }
		if (i <= j) {
			swap(&array[i], &array[j]);
			i++;
			j--;
		}
	} while (i <= j);
	sort(array, j + 1);
	sort(array + i, num - i);
	return;
}


void permutation(int* transpos, int num,int k_perm) {
	for (int k = 0; k < k_perm; k++) {
		
		for (int i = num - 2; i > -1; i--) {
			if (transpos[i] < transpos[i + 1]) {
				int min_loc = i + 1;
				for (int j = i + 1; j < num; j++) {
					if ((transpos[j] < transpos[min_loc]) && (transpos[j] > transpos[i]))
 { min_loc = j;	}
				}
				swap(&transpos[i], &transpos[min_loc]);
				sort(transpos + i + 1, num - i - 1);
				break;
			}  if( i == 0 ){ return; }
		}
		for (int i = 0; i < num; i++) {
			printf("%d", transpos[i]);
		}
		printf("\n");
	}
	return;
}

bool input_check(int* tr,int num) {
	int conc = 0;
	for (int i = 0; i < num; i++) {
		if (tr[i] == tr[i + 1]) {
			conc++;
		}
		if ((tr[i] > 9) || (tr[i] < 0)) {
			return 1;
		}
	}
	if (conc + 1 == num) { 
		return 1;
	}

	return 0;
}
int main() {
	int transpos[10];
	int num_trans = 0;
	int null_count = 0;
	char ch = getchar();
	
	while (ch == '0') {
		fprintf(stderr, "%d ", ch); ch = getchar(); null_count++; 
	}
	while (ch != '\n' && ch != '\0' && ch != EOF) {
		fprintf(stderr, "\n%d\n", ch);
		transpos[num_trans] = ch -'0';
		num_trans++;
		ch = getchar();
	}
	int k_perm;
	scanf_s("%d", &k_perm);

	if (input_check(transpos, num_trans) ) {
		printf("bad input");
		return 0;
	}
	
	permutation(transpos, num_trans, k_perm);
	return 0;
}
