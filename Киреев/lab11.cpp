#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <conio.h>
#include <iostream>
using namespace std;


FILE *openInputStream() {//особая магия
	FILE *stream;
	return stream = fopen("in.txt", "r");
}

string readTemp(string str, FILE *stream) {
	while (true)
	{
		char tmpCh;
		fscanf(stream, "%c", &tmpCh);
		if (tmpCh == '\n') return str;
		str += tmpCh;
	}
	return str;
}

string readStr(string str, FILE *stream) {
	while (true)
	{
		char tmpCh;
		fscanf(stream, "%c", &tmpCh);
		if (feof(stream)) {
			fclose(stream);
			return str;
		}
		str+=tmpCh;
	}
	return str;
}
void printString(string str) {
	for (int i = 0; i < str.length(); ++i) {
		printf_s("%c", str[i]);
	}
}
int hashStr(string str, char first, char last) {
	int result= 0;
	for (int i = first; i <= last; ++i) {
		if (str[i] >= 0) {
			result += (str[i] % 3)*(pow(3, i - first));
		}
		if (str[i] < 0) {
			result += ((str[i]+127) % 3)*(pow(3, i - first));
		}
	}
	return result;
}
void writeResult(unsigned int firstChar,unsigned int lastChar) {
	unsigned int tmp = firstChar;
	printf_s("%i ", tmp + 1);
	while (tmp != lastChar)
	{
		printf_s("%i ", tmp + 2);
		++tmp;
	}
}
void findTemplate(string temp, string str) {
	unsigned int tempHash = hashStr(temp, 0, temp.length() - 1);
	printf_s("%i \n", tempHash);

	unsigned int firstChar = 0, lastChar = temp.length() - 1,strHash = hashStr(str, firstChar, lastChar);

	while (lastChar != str.length()) {
		if (tempHash == strHash && temp[0]==str[firstChar]) {
			writeResult(firstChar, lastChar);
		}
		++firstChar;
		++lastChar;
		strHash = hashStr(str, firstChar, lastChar);
	}
}
void main() {
	string temp, str;
	FILE *stream = openInputStream();

	temp = readTemp(temp, stream);
	str = readStr(str, stream);

	findTemplate(temp, str);
}
