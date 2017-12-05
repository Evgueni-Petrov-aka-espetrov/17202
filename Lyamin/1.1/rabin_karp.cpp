#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

using namespace std;

enum { MAX_SAMPLE_SIZE = 16 };
enum { MAX_SIZE = 256 };

struct karp {
	char* arr;
	int num;
};

char getNextChar(FILE *stream) {
	char buffer;
	fscanf(stream, "%c", &buffer);
		return buffer;
}

int pow3(int i) {
	if (i == 0) {
		return 1;
	}

	if (i > 0) {
		return pow3(i - 1)*3;
	}

	if (i < 0) {
		return -1;
	}
}

int elementHash(char ch, int power) {
	if (ch >= 0) {
		return (ch % 3) * pow3(power);
	}
	else {
		return ((ch + 127) % 3) * pow3(power);
	}
}

int hashCounter(karp* str) {
	int result = 0;
	for (int i = 0; i < str->num; ++i) {
		result += elementHash(str->arr[i], i);
	}
	return result;
}

karp* SampleReading(FILE *stream, int length) {
	karp *str = (karp*)malloc(sizeof(karp));
	str->arr = (char*)malloc(length * sizeof(int));
	int i = 0;
	while (i < length && !feof(stream)) {
		char buffer = getNextChar(stream);
		if (buffer == '\n') {
			break;
		}
		str->arr[i] = buffer;
		++i;
	}
	str->num = i;
	return str;
}

karp* StrReading(FILE *stream, int length) {
	karp *str = (karp*)malloc(sizeof(karp));
	str->arr = (char*)malloc(length * sizeof(int));
	int i = 0;
	while (i < length && !feof(stream)) {
		char buffer = getNextChar(stream);
		str->arr[i] = buffer;
		++i;
	}
	str->num = i;
	return str;
}

karp* getNextStr(karp* str, char nextChar) {
	karp* str2 = str;
	for (int i = 1; i < str->num; ++i) {
		str->arr[i - 1] = str2->arr[i];
	}
	str->arr[str->num - 1] = nextChar;;
	return str;
}

int reHash(int PrevHash, char nextChar, char prevChar, int lenght) {
	return (PrevHash - elementHash(prevChar, 0) + 1) / 3 + elementHash(nextChar, lenght - 1);
}

int pushBack(int *result, int num, int pointer) {
	*result = num;
	return pointer + 1;
}

void print(int *result, int num) {
	for (int i = 0; i < num; ++i) {
		cout << result[i];
		if (i == num - 1) {
			break;
		}
		cout << ' ';
	}
	return;
}

void KarpRabin(int *result, karp *sample, karp *str, int sampleHash, int strHash, int *counter, int *pointer) {
	if (sampleHash == strHash) {
		(*pointer) = pushBack(&result[*pointer], *counter, *pointer);
		if (str->arr[0] == sample->arr[0]) {
			for (int i = 1; i < sample->num; ++i) {
				if (elementHash(str->arr[i], 0) != elementHash(sample->arr[i], 0)) {
					break;
				}
				(*pointer) = pushBack(&result[*pointer], *counter + i, *pointer);
			}
		}
	}
}

void RabinKarp() {
	FILE *stream;
	stream = fopen("in.txt", "r");

	int* result = (int*)malloc(sizeof(int)*MAX_SIZE);

	karp* sample = SampleReading(stream, MAX_SAMPLE_SIZE + 1);
	karp* str = StrReading(stream, sample->num);

	int sampleHash = hashCounter(sample);
	int strHash = hashCounter(str);
	int counter = 1; //begin from 1
	int pointer = 0;
	pointer = pushBack(result, sampleHash, pointer);

	while (!feof(stream)) {
		KarpRabin(result, sample, str, sampleHash, strHash, &counter, &pointer);
		char prevChar = str->arr[0];
		char nextChar = getNextChar(stream);
		str = getNextStr(str, nextChar);
		strHash = reHash(strHash, nextChar, prevChar, str->num);
		++counter;
	}
	print(result, pointer);
}

int main() {
	RabinKarp();
	system("pause");
}
