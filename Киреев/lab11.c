#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <conio.h>
using namespace std;
FILE *openInputStream() {//особая магия
	FILE *stream;
	return stream = fopen("in.txt", "r");
}

char rusLang(char tmpCh) {// колдовство
	return tmpCh + 96 + 224;
}

string readStr(string str, FILE *stream) {
	while (true)
	{
		char tmpCh;
		fscanf(stream, "%c", &tmpCh);
		//if (tmpCh < 0) { tmpCh = rusLang(tmpCh);}если русские символы, то колдуем
		if (tmpCh == '\n') return str;
		if (feof(stream)) {
			fclose(stream);
			return str;
		}
		str += tmpCh;
	}
	return str;
}
void printString(string str) {
	for (char i = 0; i < str.length(); ++i) {
		printf_s("%c", str[i]);
	}
}
int hashStr(string str, char first, char last) {
	int result= 0;
	for (char i = first; i <= last; ++i) {
		result += (str[i] % 3)*(pow(3, i-first));
	}
	return result;
}
void writeResult(char firstChar, char lastChar) {
	char tmp = firstChar;
	printf_s("%i ", tmp + 1);
	while (tmp != lastChar)
	{
		printf_s("%i ", tmp + 2);
		++tmp;
	}
}
void findTemplate(string temp, string str) {
	int tempHash = hashStr(temp, 0, temp.length() - 1);
	printf_s("%i \n", tempHash);

	char firstChar = 0, lastChar = temp.length() - 1;
	int strHash = hashStr(str, firstChar, lastChar);

	while (lastChar != str.length()) {
		if (tempHash == strHash) {
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

	temp = readStr(temp, stream);
	str = readStr(str, stream);
	findTemplate(temp, str);
	system("pause");
}