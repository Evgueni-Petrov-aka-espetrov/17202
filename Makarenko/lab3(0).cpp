#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
using namespace std;
FILE* input = fopen("in.txt", "r");
FILE* output = fopen("out.txt", "w");

void createheap(int *str, int size, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if (left < size && str[left] > str[largest]) {
		largest = left;
	}
	if (right < size && str[right] > str[largest]) {
		largest =right;
	}
	if (largest != i) {
		swap(str[i], str[largest]);
		createheap(str, size, largest);
	}
}
void heapsort(int*str, int size) {
	for (int i = (size / 2) - 1; i >= 0; --i) {
		createheap(str, size, i);
	}
	for (int i = size - 1; i >= 0; i--) {
		swap(str[0], str[i]);
		createheap(str, i, 0);
	}

}
void result(int*str, int size) {
	for (int i = 0; i < size; i++) {
		fprintf(output, "%d ", str[i]);
	}

}



int main() {
	int quantity;
	fscanf_s(input, "%d", &quantity);
	if (quantity <= 0) {
		return 0;
	}
	int* str = (int*)malloc(sizeof(int)*quantity);
	for (int i = 0; i < quantity; i++) {
		fscanf_s(input, "%d", &str[i]);
	}
	heapsort(str, quantity);
	result(str, quantity);
	return 0;
}