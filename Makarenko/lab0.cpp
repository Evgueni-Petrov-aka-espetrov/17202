#include <iostream>
#include <fstream>
using namespace std;
enum{max=13};
struct sdata {
	char number[max];
	int locationofdot;
	int quantity;
};
//перевод из символа в целое число
int CharToInt(char c) {
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
char IntToChar(int i) {
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
//нахождение количества элементов в строке
int Quantity(sdata str, int base) {
	for (int counter = 0; counter < max; ++counter) {
		if ((CharToInt(str.number[counter]) < 0 || CharToInt(str.number[counter]) >= base) && str.number[counter] != '.') {
			return counter;
		}
	}
	return max;
}
//нахождение количества точек
int Countdot(sdata str) {
	int quantity = 0;
	for (int i = 0; i <str.quantity; i++) {
		if (str.number[i] == '.') {
			++quantity;
		}
	}

	return quantity;
}
//поиск положения точки
int lookfordot(sdata str) { 
	for (int i = 0; i < str.quantity; i++) {
		if (str.number[i] == '.') {
			if (i < 13) {
				return i;
			}
			else
			{
				return str.quantity;
			}
		}
	}
	}
//перевод из 1-ой системы счисления в десятичную(целая часть)
long long ConvertToDec1(sdata str, int b1) {
	long long result = 0;
	if (str.locationofdot == 13) {
		str.locationofdot = str.quantity;
	}
	for (int i = 0; i < str.locationofdot; ++i) {
		result = result*b1 + CharToInt(str.number[i]);
	}
	return result;
}
//перевод из 1-ой системы счисления в десятичную(дробная часть)
double ConvertToDec2(sdata str, int b1) {
	double result = 0;
	for (int i = str.quantity - 1; i > str.locationofdot; --i) {
			result = (result + CharToInt(str.number[i])) / b1;
		}
	
	return result;
}
//перевод из десятичной системы счисления во 2-ую с.с(целая часть)
void Convertto2base1(long long Dec1, int b2) {
	char Cellb2[52] = {};
	if (Dec1 == 0) {
		cout << '0';
	}
	else {
		int count = 0;
		while (Dec1 > 0) {
			Cellb2[count] = IntToChar(Dec1%b2);
			++count;
			Dec1 /= b2;
		}
		for (int i = count - 1; i >= 0; --i) {
			cout << Cellb2[i];
		}
	}
	return;
}
//перевод из десятичной системы счисления во 2-ую с.с(дробнаячасть)
void Convertto2base2(double Dec2, int b2) {
	int count = 0;
	if (Dec2 == 0) {
		cout << '0';
		return;
	}
	else {
		while ((count < max - 1) && (Dec2 != 0)) {
			int tmp = Dec2 * b2;
			Dec2 = Dec2*b2 - tmp;
			cout << IntToChar(tmp);
		}
	}
	return;
}
//проверка на правильность ввода исходных данных
int verification(int b1, int b2, sdata str) {
	if (Countdot(str) > 1) {
		return 0;
	}
	if ((b1 < 2) || (b2 < 2) || (b1 > 16) || (b2 > 16)) {
		return 0;
	}
	if (lookfordot(str) == 0 || lookfordot(str) == str.quantity - 1) {
		return 0;
	}
	return 1;
}
//вывод результата 
void output(sdata str, int b1, int b2) {
	if (verification(b1, b2, str) == 0) {
		cout << "bad input";
		return;
	}
	if (b1 == b2) {
		for (int i = 0; i < str.quantity; i++) {
			cout << str.number[i];
		}
	}
	else {
		Convertto2base1(ConvertToDec1(str, b1), b2);
		if (lookfordot(str) != str.quantity) {
			cout << '.';
			Convertto2base2(ConvertToDec2(str, b1), b2);
			return;
		}
	}
}

void main() {
	ifstream str("in.txt");
	int b1, b2;
	str >> b1 >> b2;
	sdata primary;
	primary.quantity = 0;
	while (primary.quantity < max) {
		str >> primary.number[primary.quantity];          
		if (primary.number[primary.quantity] == '\0') {
			break;
		}
		++primary.quantity;
	}
    str.close();
	primary.locationofdot = lookfordot(primary);//находим местоположение точки
	primary.quantity = Quantity(primary, b1);//находим количество элементов в строке
	output(primary, b1, b2); //выводим результат
}
