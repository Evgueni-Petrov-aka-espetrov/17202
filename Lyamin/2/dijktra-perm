#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>

using namespace std;

enum {MAX_SIZE = 10};

struct mass {
	int* arr;
	int count;
};

void swap(int *a, int *b) {
	int s = *a;
	*a = *b;
	*b = s;
}

char getNextChar(FILE* stream) {
	char buffer;
	fscanf(stream, "%c", &buffer);
	return buffer;
}

int charToInt(char ch) {
	return ch - '0';
}

int check1(mass* perm) {
	for (int i = 0; i < perm->count; ++i) {
		if (perm->arr[i] > 9 || perm->arr[i] < 0) {
			return 0;
		}
	}
		return 1;
}

int check2(mass* perm) {
	int a[10] = {};
	for (int i = 0; i < perm->count; ++i) {
		++a[perm->arr[i]];
	}

	for (int i = 0; i < 10; ++i) {
		if (a[i] > 2) {
			return 0;
		}
	}
	return 1;
  
}
int dijktra(mass* perm) {
	int pointer = perm->count - 2;
	while (perm->arr[pointer] > perm->arr[pointer + 1] && pointer != -1) { --pointer; }
	if (pointer == -1) { 
		return 0; 
	}

	for (int i = perm->count - 1; i > pointer; --i) {
		if (perm->arr[i] > perm->arr[pointer])
		{
			swap(&perm->arr[i], &perm->arr[pointer]);
			break;
		}
	}

	for (int i = perm->count - 1; i > pointer; --i) {
		for (int j = i; j > pointer; --j) {
			if (perm->arr[i] < perm->arr[j])
				swap(&perm->arr[i], &perm->arr[j]);
		}
	}
	return 1;
}

mass* reading(FILE* stream) {
	int length = 0;
	mass* perm = (mass*)malloc(sizeof(mass));
	perm->arr = (int*)malloc(sizeof(int)*MAX_SIZE);

	while (length < MAX_SIZE) {
		char ch = getNextChar(stream);
		if (ch == '\n') {
			break;
		}
		perm->arr[length] = charToInt(ch);
		++length;
	}
	perm->count = length;
	return perm;
}

void printing(mass* perm) {
	for (int i = 0; i < perm->count; ++i) {
		cout << perm->arr[i];
		}
	cout << endl;
}

int main() {
	FILE* stream = fopen("in.txt", "r");
	mass* perm = reading(stream);

	int count;
	fscanf(stream, "%d", &count);
	if (check1(perm) == 0) {
		cout << "bad input";
		return 0;
	}

	if (check2(perm) == 0) {
		cout << "bad input";
		return 0;
	}

	for (int i = 0; i < count; ++i){
		if (dijktra(perm) == 0) {
			break;
		} else {
			printing(perm);
		}
	}
	return 0;
}
