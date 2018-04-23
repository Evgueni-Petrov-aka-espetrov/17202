#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>

using namespace std;

enum { MaxSampleSize = 16 };
enum { MaxSize = 256 };

const int degreeof3[] = { 1,3,9,27,81,243,729,2187,6561,19683,59049,59049 * 3,59049 * 9,59049 * 27,59049 * 81,59049 * 243,59049 * 729 };


struct Data{
	char* str;
	int quantity;
};
char getNextChar(FILE *input) {
	char buffer;
	fscanf(input, "%c", &buffer);
	return buffer;
}
int powthree(int i) {
	if (i < 0) {
		return -1;
	}
	else{
		return degreeof3[i];
	}

}
int HashOfChar(char c, int power) {
	if (c >= 0) {
		return (c % 3)*powthree(power);
	}
	else{
		return ((c + 127) % 3)*powthree(power);
	}
}
int Hash(Data* str) {
	int ResultOfHash = 0;
	for (int i = 0; i < str->quantity; i++) {
		ResultOfHash += HashOfChar(str->str[i], i);
	}
	return ResultOfHash;
}
Data* GetSample(FILE* input, int lenght) {
	Data *str = (Data*)malloc(sizeof(Data));
	str->str = (char*)malloc(lenght * sizeof(int));
	int i = 0;
	while (i < lenght && !feof(input)){
		char tmp = getNextChar(input);
		if (tmp == '\n') {
			break;
		}
		str->str[i] = tmp;
		i++;
	}
	str->quantity = i;
	return str;
}
Data* GetStr(FILE* input, int lenght) {
	Data *str = (Data*)malloc(sizeof(Data));
	str->str = (char*)malloc(lenght * sizeof(int));
	int i = 0;
	while (i < lenght && !feof(input)) {
		char tmp = getNextChar(input);
		if (tmp == '\0') { 
			break; 
		}
		str->str[i] = tmp;
		++i;
	}
	str->quantity = i;
	return str;
}
Data* GetNext(Data* str, char Next) {
	Data* otherstr = str;
	for (int i = 1; i < str->quantity; i++) {
		str->str[i - 1] = otherstr->str[i];
	}
	str->str[str->quantity - 1] = Next;
	return str;
}

int ReHash(int PreviousHash, char Next, char PreviousChar, int lenght) {
	return(PreviousHash - HashOfChar(PreviousChar, 0) + 1) / 3 + HashOfChar(Next, lenght - 1);
}

void Printing(int *result,int quantity,FILE* output) {
	
	for (int i = 0; i < quantity; i += 4) {
		if (i > quantity - 3) {
			for (int j = i; j < quantity; j++) {
				fprintf_s(output, "%d ", result[j]);
			}
		}
		else {
			fprintf_s(output, "%d %d %d %d", result[i], result[i + 1], result[i + 2], result[i + 3]);
			fprintf_s(output, "%c", ' ');
		}
	}
	return;
}
void Processing(int *result, Data* sample, Data* str, int sampleHash, int strHash, int counter, int *pointer) {
	if (sampleHash == strHash) {
		result[*pointer] = counter;
		*pointer += 1;
		if (str->str[0] == sample->str[0]) {
			for (int i = 1; i < sample->quantity; i++){
				if (HashOfChar(str->str[i], 0) != HashOfChar(sample->str[i],0)) {
					break;
				}
				
				result[*pointer] = counter + i;
				*pointer += 1;
			}
		}
	}

}
void RabinKarp(FILE* input, FILE* output) {
	int *result = (int*)malloc(sizeof(int)*MaxSize);
	Data* sample = GetSample(input, MaxSampleSize + 1);
	Data* str = GetStr(input, sample->quantity);
	int sampleHash = Hash(sample);
	int strHash = Hash(str);
	int counter = 1;
	int pointer = 1;
	result[0] = sampleHash;
	while (true){
		if (feof(input)) {
			break;
		}
		Processing(result, sample, str, sampleHash, strHash, counter, &pointer);
		char PreviousChar = str->str[0];
		char Next = getc(input);
		str = GetNext(str, Next);
		strHash = ReHash(strHash, Next, PreviousChar, str->quantity);
		++counter;
		}
	Printing(result, pointer, output);
	
}
int main() {
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	RabinKarp(input, output);
	return 0;
}
