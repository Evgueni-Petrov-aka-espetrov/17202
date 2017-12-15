#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

enum { MAX_SAMPLE_SIZE = 16};
enum { MAX_SIZE = 256 };

struct moore {
	char* arr;
	int num;
};

char getNextChar(FILE *stream) {
	return fgetc(stream);
}

moore* readSample(FILE* stream) {
	moore* sample = (moore*)malloc(sizeof(moore));
	sample->arr = (char*)malloc(sizeof(char)*(MAX_SAMPLE_SIZE + 1));

	int i = -1;
	while (i < MAX_SAMPLE_SIZE) {
		++i;
		char buffer = getNextChar(stream);
		if (buffer == '\n') {
			break;
		}
		sample->arr[i] = buffer;
	}
	sample->num = i;
	return sample;
}

moore* readStr(FILE* stream, int length) {
	moore* str = (moore*)malloc(sizeof(moore));
	str->arr = (char*)malloc(sizeof(char)*length);
	int i = 0;
	while (i < length) {
		char buffer = getNextChar(stream);
		str->arr[i] = buffer;
		++i;
	}
	str->num = i;
	return str;
}

void getNextStr(FILE* stream, moore* str, int movement){
	char* buffer = (char*)malloc(sizeof(char)*movement);
	char* buffer2 = (char*)malloc(sizeof(char)*str->num);

	int count = 0;
	while (count < movement && !feof(stream)) {
		buffer[count] = getNextChar(stream);
		++count;
	}

	movement = count;
	for (int i = 0; i <= str->num - 1 - movement ; ++i) {
		buffer2[i] = str->arr[i + movement];
	}
	for (int i = 0; i <= movement; ++i) {
		buffer2[str->num + i - movement] = buffer[i];
	}

	free(buffer);
	free(str->arr);
	str->arr = buffer2;
	return;
}

int set(const moore* sample,const int* offset, char ch) {
	int check = 0;
	for (int i = 0; i < sample->num; ++i) {
		if (sample->arr[i] == ch) {
			if (offset[i] - 1 == 0) {
				return offset[check];
			} else {
				return offset[i] - 1;
			}
		}
	}
	return offset[check];
}

int* offset(moore* sample) {
	int* result = (int*)malloc(sizeof(int)*(sample->num));
	for (int i = 0; i < sample->num + 1; ++i) {
		result[i] = sample->num - i;
	}
	return result;
}

void MooreBoyer(FILE* stream, FILE* fout, static const moore* sample, moore* str, static const int* move, int* counter) {
	int movement;
	int c = *counter;

	for (int i = sample->num - 1; i >= 0; --i) {
		fprintf(fout, "%d ", c + i);
		if (sample->arr[i] == str->arr[i]) {
			continue;
		} else {
			movement = set(sample, move, str->arr[str->num - 1]);
			c += movement;
			*counter = c;
			getNextStr(stream, str, movement);
			return;
		}
	}

	movement = set(sample, move, str->arr[str->num - 1]);
	c += movement;
	*counter = c;
	getNextStr(stream, str, movement);
	return;
}

void BoyerMoore() {
	FILE* stream = fopen("in.txt", "r");
	FILE* fout = fopen("out.txt", "w");
	moore* sample = readSample(stream);
	moore* str = readStr(stream, sample->num);

	const int* move = offset(sample);
	int counter = 1;
	while (!feof(stream)) {
		MooreBoyer(stream, fout, sample, str, move, &counter);
	}

	free(sample);
	free(str);
	fclose(stream);
	fclose(fout);
	return;
}

int main() {
	BoyerMoore();
	return 0;
}
