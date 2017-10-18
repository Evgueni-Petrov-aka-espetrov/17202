#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <conio.h>
#include <iostream>
#include <vector>
using namespace std;

FILE *openInputStream() {//особая магия
	FILE *stream;
	return stream = fopen("in.txt", "r");
}
string readTempFromStream(FILE *stream) {
	string str;
	while (true) {
		char tmpCh;
		tmpCh = getc(stream);
		if (tmpCh == '\n') { break; }
		str += tmpCh;
	}
	return str;
}
string readStrFromStream(FILE *stream, const int length) {
	string str;
	int i = 0;
	while (i < length) {
		char tmpCh;
		tmpCh = getc(stream);
		str += tmpCh;
		++i;
	}
	return str;
}
char readNextCh(FILE *stream) {
	char tmpCh;
	tmpCh = getc(stream);
	return tmpCh;
}
string changeStr(const string &str, const char nextChar) {
	return str.substr(1) + nextChar;
}

int hashStr(const string &str, const vector<int> &pows) {
	int result = 0;
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] >= 0) {
			result += (str[i] % 3)*pows[i];
		}
		if (str[i] < 0) {
			result += ((str[i] + 127) % 3)*pows[i];
		}
	}
	return result;
}

int hashNextStr(const string &str, const int prevHash, const char nextChar, const vector<int> &pows) {
	if (str[0] >= 0 && nextChar >=0) {
		return (prevHash - (str[0] % 3)*pows[0]) / 3 + (nextChar % 3)*pows[str.length() - 1];
	}
	if (str[0] < 0 && nextChar >= 0) {
		return (prevHash - ((str[0]+127) % 3)*pows[0]) / 3 + (nextChar % 3)*pows[str.length() - 1];
	}
	if (str[0] >= 0 && nextChar < 0) {
		return (prevHash - (str[0] % 3)*pows[0]) / 3 + ((nextChar + 127) % 3)*pows[str.length() - 1];
	}
	if (str[0] < 0 && nextChar < 0) {
		return (prevHash - ((str[0] + 127) % 3)*pows[0]) / 3 + ((nextChar+127) % 3)*pows[str.length() - 1];
	}
}

void writeResult(const unsigned int firstChar, const unsigned int lastChar, const string &str, const string &temp) {
	unsigned int tmp = firstChar;
	printf_s("%i ", tmp + 1);
	int i = 0;
	while (tmp != lastChar)
	{
		if (temp[i] != str[i]) {
			break;
		}
		printf_s("%i ", tmp + 2);
		++tmp;
		++i;
	}
}

void RabinKarp(FILE* stream, string str, const string &temp) {


	int firstChar = 0, lastChar = temp.length() - 1;

	vector<int> pows(temp.length());
	for (int i = 0; i < temp.length(); ++i) {
		pows[i] = pow(3, i);
	}
	int tempHash = hashStr(temp, pows);
	printf_s("%i ", tempHash);
	int strHash = hashStr(str, pows);
	while (!feof(stream))
	{
		if (tempHash == strHash) {
			writeResult(firstChar, lastChar, str, temp);
		}
		char nextChar = readNextCh(stream);
		strHash = hashNextStr(str, strHash, nextChar, pows);
		str = changeStr(str, nextChar);
		//cout << strHash << ";";
		/*strHash = hashStr(str, pows);*/
		++firstChar;
		++lastChar;
	}
}
void main() {
	FILE *stream = openInputStream();
	string temp = readTempFromStream(stream), str = readStrFromStream(stream, temp.length());
	RabinKarp(stream, str, temp);
	//system("pause");
}
