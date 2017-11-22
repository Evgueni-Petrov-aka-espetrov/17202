#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include <algorithm>
using namespace std;
FILE* openInputStream() {
	FILE* stream = fopen("in.txt", "r");
	return stream;
}
void swap(char *a, char *b) {
	char buffer = *a;
	*a = *b;
	*b = buffer;
}
int charToInt(const char c) {
	const char digit[] =
	{ '0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
		};
	for (int j = 0; j < 10; ++j) {
		if (digit[j] == c) {
			return j;
		}
	}
	return -1;
}
bool checkCh(char ch) {
	if (charToInt(ch) == -1) {
		return false;
	}
	return true;
}
bool readAndCheckArr(FILE* stream, char* arr, char *last) {
	bool isUsed[10];
	for (int i = 0; i < 10; ++i) {
		isUsed[i] = false;
	}
	char tmp = getc(stream), i = 0;
	while (tmp != '\n') {
		if (!checkCh(tmp)) {
			printf("bad input");
			return false;
		}
		arr[i] = tmp;
		if (isUsed[charToInt(tmp)]==true) {
			printf("bad input");
			return false;
		}
		isUsed[charToInt(tmp)] = true;
		++i;
		tmp = getc(stream);
	}
	*last = i;
	return true;
}
void printArr(const char* arr) {
	for (int i = 0; i < 10; ++i) {
		if(charToInt(arr[i])!=-1)
			printf("%d", charToInt(arr[i]));
	}
	printf("\n");
}
void sortArr(char * arr, const int from, const int to) {
	for (int i = from; i < to; ++i) {
		for (int j = from; j < to; ++j) {
			if (arr[i] < arr[j]) {
				swap(&arr[i], &arr[j]);
				break;
			}
		}
	}
}
void dijkstraPerm(char * arr, const int pos, const int numberOfPerm, const int last) {
	if (pos == 0 || numberOfPerm == 0) {
		return;
	}
	if (arr[pos] > arr[pos - 1]) {
		for (int i = last-1; i > pos-1; --i) {
			if (arr[i] > arr[pos-1]) {
				swap(&arr[pos - 1], &arr[i]);
				break;
			}
		}
		sortArr(&arr[0], pos, last);
		printArr(&arr[0]);
		dijkstraPerm(&arr[0], last-1, numberOfPerm-1, last);
	}
	else {
		dijkstraPerm(&arr[0], pos-1, numberOfPerm, last);
	}
}
void main() {
	FILE* stream = openInputStream();
	char arr[10], last;
	if (!readAndCheckArr(stream, &arr[0], &last)) {
		return;
	}
	int numberOfPerm;
	fscanf(stream, "%d", &numberOfPerm);
	dijkstraPerm(&arr[0], last-1 , numberOfPerm, last);
}
