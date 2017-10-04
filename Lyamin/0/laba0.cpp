#pragma message("\n\nUSE WITH -m 2600\n\n")

#include <iostream>
#include <fstream>

using namespace std;
enum { MAX_STR = 13 };
struct Massive{
	char adr[MAX_STR];
	int PointLoc;
	int num;
};
static const char alphavite[] = "0123456789ABCDEFabcdef";
//from char to imt
int CharToInt(char num) {
	const char* pos = strchr(alphavite, num);
	if (pos == NULL) {
		return -1;
	}
	return pos - alphavite < 16 ? pos - alphavite : pos - alphavite - 6;
}
//from int to char
char IntToChar(int num) {
	return alphavite[num];
}
//count of dots in original number
int CountPoint(Massive a) {
	int result = 0;
	for (int i = 0; i < a.num; ++i) {
		if (a.adr[i] == '.') {
			++result;
		}
	}
	return result;
}
//return dot's position
int SearchPoint(Massive a) {
	for (int i = 0; i < a.num; ++i) {
		if (a.adr[i] == '.') {
			return i;
		}
	}
	return a.num;
}
//return number of "good" elements in original number
int Number(Massive a, int base) {
	for (int count = 0; count < MAX_STR; ++count) {
		if ((CharToInt(a.adr[count]) < 0 || CharToInt(a.adr[count]) >= base) && a.adr[count] != '.') {
			return count;
		}
	}
	return MAX_STR;
}
//from original number to dec floor
long long DecCell(Massive a, int b1) {
	long long result = 0;
	for (int i = 0; i < a.PointLoc; ++i) {
		result = result*b1 + CharToInt(a.adr[i]);
	}
	return result;
}
//from original number to dec cell
double DecFloor(Massive a, int b1) {
	double result = 0;
	if (CountPoint(a) == 0) {
		result = -1;
	}
	else {
		for (int i = a.num - 1; i > a.PointLoc; --i) {
			result = (result + CharToInt(a.adr[i])) / b1;
		}
	}
	return result;
}
//printing dec cell to b2
void Cell2b2(long long CellDec, int b2) {
	char Cellb2[52] = {};
	if (CellDec == 0){
		cout << '0';
	}
	else {
		int count = 0;
		while (CellDec > 0) {
			Cellb2[count] = IntToChar(CellDec%b2);
			++count;
			CellDec /= b2;
		}
		for (int i = count - 1; i >= 0; --i) {
			cout <<Cellb2[i];
		}
	}
	return;
}
//printing dec floor to b2
void Floor2b2(double FloorDec, int b2) {
	int count = 0;
	if (FloorDec == 0) {
		cout << '0';
		return;
	}
	else {
		while ((count < MAX_STR - 1) && (FloorDec != 0)) {
			int a = FloorDec * b2;
			FloorDec = FloorDec*b2 - a;
			cout << IntToChar(a);
		}
	}
	return;
}
//checking bad input
int Checking(Massive a, int b1, int b2) {
	//c.c. doesn't match//
	if ((b1 < 2) || (b1 > 16) || (b2 < 2) || (b2 > 16)) {
		return 0;
	}

	//more then 1 dot in original//
	if (CountPoint(a) > 1) {
		return 0;
	}

	//dot on the first or the last place//
	if ((SearchPoint(a) == 0) || (SearchPoint(a) == a.num - 1)) {
		return 0;
	}
	
	return 1;
}
//PRINTING
void Printing(Massive a, int b1, int b2) {
	if (Checking(a, b1, b2) == 0) {
		cout << "bad input";
		return;
	}
	else {
		if (b1 == b2) {
			for (int i = 0; i < a.num; ++i) {
				cout << a.adr[i];
			}
			return;
		}
		else{
			Cell2b2(DecCell(a, b1), b2);
			if (SearchPoint(a) != a.num) {
				cout << '.';
				Floor2b2(DecFloor(a, b1), b2);
				return;
			}
		}	
	}
}

int main() {
	ifstream g("in.txt");
	int b1, b2;
	g >> b1 >> b2;
	Massive original;
	original.num = 0;
	while (original.num < MAX_STR) {
		g >> original.adr[original.num];
		++original.num;
	}
	g.close();

	original.num = Number(original, b1);
	original.PointLoc = SearchPoint(original);
	Printing(original, b1, b2);
	return 0;
}
