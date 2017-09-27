// blablablabla
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// dont test me. Im not ready
//перевод из символа в целое число
int charToInt(char c) {
	const char digit[] =
	{ '0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E',
		'F', 'a', 'b', 'c', 'd',
		'e', 'f' };
	for (int j = 0; j < 22; ++j) {
		if (digit[j] == c) {
			if (j < 16)
				return j;
			if (j >= 16)
				return j - 6;
		}
	}
	return -1;
}
//перевод из целого числа в символ
char intToChar(int i) {
	const char digit[] =
	{ '0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E',
		'F', 'a', 'b', 'c', 'd',
		'e', 'f' };
	if (i >= 0 && i <= 15)
	{
		return digit[i];
	}
	return -1;
}
//функция проверяет введённое число на соответствие с.с. b1
bool isValid(const string &number, int b1, int dot) {
	if (number.empty()) {
		return false;
	}
	if (dot != number.rfind('.'))
	{
		return false;
	}
	for (int i = 0; i < number.length(); ++i) {
		if (charToInt(number[i]) >= b1) {
			return false;
		}
	}
	if (dot == 0 || dot == number.length() - 1) { // espetrov: объедините эти два return?
		return false;
	}
	return true;
}
//функция принимает введённое число и выдаёт целую часть в десятичном виде
long long strToDec_cell(string &str, int b1, int dot) {
	long long result = 0;
	if (dot > 0) {
		for (int i = 0; i < dot; i++)
		{
			result = result*b1 + charToInt(str[i]);
		}
	}
	else { // espetrov: объединить с веткой dot > 0
		for (int i = 0; i < str.length(); i++)
		{
			result = result*b1 + charToInt(str[i]);
		}
	}
	return result;
}
//функция принимает введённое число и выдаёт дробную часть в десятичном виде
double strToDec_float(string &str, int b1, int dot) {
	if (dot < 0) {
		return 0;
	}
	double result = 0;
	for (int i = str.length() - 1; i >= dot + 1; i--)
	{
		result = result / b1 + charToInt(str[i]);
	}
	result = result / b1; // espetrov: объединить с return
	return result;
}
//функция принимает целую часть числа в десятичном виде
//выдаёт целую часть числа в нужной с.с в виде строки
string decToB2_cell(long long cell_part, int b2) {
	if (cell_part == 0) {
		return "0";
	}
	string result;
	while (cell_part%b2 != 0 || cell_part / b2 != 0) { // espetrov: упростите условие?
		//если остаток от деления числа на b2 = 0 и 
		//деление нацело на b2 = 0, то завершаем цикл
		result += intToChar(cell_part%b2);
		//делим число нацело
		cell_part = cell_part / b2;
	}
	return result;
}
//функция принимает дробную часть числа в десятичном виде
//выдаёт дробную часть числа в нужной с.с в виде строки
string decToB2_float(double float_part, int b2) {
	string result;
	for (int i = 0; i <= 12; ++i)
	{
		//если кол-во цифр полсле запятой > 12 или дробная часть стала равно нулю
		//то выходим из цикла
		if (float_part == 0) {
			break;
		}
		result += intToChar((int)(float_part*b2));
		//домножаем на основание новой системы счисления и отнимаем целую часть 
		float_part = float_part*b2 - (int)(float_part*b2);
	}
	return result;
}
//функция принимает целую и дробную часть числа в десятичном виде
//выдаёт результат в нужной с.с. в виде строки
string toResult(long long cell_part, double float_part, int b2) {
	string result;
	if (float_part == 0) {
		result = decToB2_cell(cell_part, b2);
		reverse(result.begin(), result.end());
	}
	else {
		result = decToB2_cell(cell_part, b2); // espetrov: не зависит от float_part == 0 -- вынесите перед if?
		reverse(result.begin(), result.end());
		result += '.' + decToB2_float(float_part, b2);
	}
	return result;
}
void main() {
	string orig; // изначальное число
	int b1 = 0, b2 = 0;

	cin >> b1 >> b2;

	//проверяем введённые системы счисления
	if (b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16) {
		cout << "bad input";
		return;
	}

	cin >> orig;

	// 0 = 0 в любой с.с.
	if (orig == "0") {
		cout << '0';
		return;
	}
	// найдём точку
	int dot = orig.find('.');
	if (isValid(orig, b1 , dot) == false) {
		cout << "bad input";
		return;
	}
	// если b1 = b2, то очевидно, что ответ - то же самое число
	if (b1 == b2) {
		cout << orig;
		return;
	}
	// целая часть началього числа в десятичном виде
	long long cell_decNum = strToDec_cell(orig, b1, dot);
	// дробная часть началього числа в десятичном виде
	double float_decNum = strToDec_float(orig, b1, dot);
	// результат
	string result = toResult(cell_decNum, float_decNum, b2);
	cout << result;
}
