#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
using namespace std;

int pivot(int left, int right) {
	return left;
}
int partion(int* str, int left, int right) {
	int l = left;
	int r = right;
	while (l <= r){
		while (str[l]<str[pivot(left,right)]){
			l = l + 1;
		}
		while (str[r] > str[pivot(left, right)]){
			r = r - 1;
		}
		if (l <= r) {
			swap(str[l], str[r]);
			l = l + 1;
			r = r - 1;
		}


	}
	return l;
}
void quicksort(int* str, int left, int right) {
	if (left < right) {
		int p = partion(str, left, right);
		quicksort(str, left, p - 1);
		quicksort(str, p, right);
	}
	return;
}

int main() {
	FILE* input=fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
    int quantity;
	fscanf_s(input, "%d", &quantity);
	if (quantity <= 0) {
		return 0;
	}
	int* str = (int*)malloc(sizeof(int)*quantity);
	for (int i = 0; i < quantity; i++) {
		fscanf_s(input, "%d", &str[i]);
	}
	quicksort(&str[0], 0, quantity - 1);
	for (int i = 0; i < quantity; i++) {
		fprintf(output, "%d ", str[i]);
     }
	return 0;
}
