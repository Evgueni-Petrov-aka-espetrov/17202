USE WITH -m 2600

#include <iostream>
#include <fstream>

using namespace std;

struct Massive{
	char *adr;
	int PointLoc;
	int num;
};
//from char to imt
int CharToInt(char *num) {
	char alphavite[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F',
		'a', 'b', 'c', 'd', 'e', 'f'};
	for (int i = 0; i < 22; ++i) {
		if (*num == alphavite[i]) {
			if (i < 16) {
				return i;
			}
			else {
				return i - 6;
			}
		}
	}
	return -1;
}
//check original number b1
int b1check(Massive a, const int b1) {
	for (int i = 0; i < a.num; ++i, ++a.adr) {
		if ((CharToInt(a.adr) >= b1) && (*a.adr != '.')) {
			return 0;
		}
	}
}
//from int to char
char IntToChar(int num) {
	char alphavite[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F',
		'a', 'b', 'c', 'd', 'e', 'f' };
	return alphavite[num];
}
//count of dots in original number
int CountPoint(Massive a) {
	int result = 0;
	for (int i = 0; i < a.num; ++i, ++a.adr) {
		if (*a.adr == '.') {
			++result;
		}
	}
	return result;
}
//return dot's position
int SearchPoint(Massive a) {
	for (int i = 0; i < a.num; ++i, ++a.adr) {
		if (*a.adr == '.') {
			return i;
		}
	}
	return a.num;
}
//return number of "good" elements in original number
int Number(char *chislo) {
	int count = 0;
	while (count < 13) {
		if ((CharToInt(chislo)>=0&& CharToInt(chislo)<=15)||*chislo=='.') {
			++count;
			++chislo;
		}
		else {
			break;
		}
	}
	return count;
}
//from original number to dec floor
long long DecCell(Massive a, const int b1) {
	long long result = 0;
	for (int i = 0; i < a.PointLoc; ++i, ++a.adr) {
		result = result*b1 + CharToInt(a.adr);
		//cout << i << ' ' << result << endl;
	}
	return result;
}
//from original number to dec cell
double DecFloor(Massive a, const int b1) {
	double result = 0;
	if (CountPoint(a) == 0) {
		result = -1;
	}
	else {
		a.adr += a.num - 1;
		for (int i = a.num - 1; i > a.PointLoc; --i, --a.adr) {
			result = (result + CharToInt(a.adr)) / b1;
		}
	}
	return result;
}
//printing dec cell to b2
void Cell2b2(long long CellDec, const int b2) {
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
void Floor2b2(double FloorDec, const int b2) {
	char Floorb2[12] = {};
	int count = 0;
	if (FloorDec == 0) {
		cout << '0';
		return;
	}
	else {
		while ((count < 12) && (FloorDec != 0)) {
			int a = FloorDec * b2;
			FloorDec = FloorDec*b2 - a;
			cout << IntToChar(a);
		}
	}
	return;
}
//checking bad input
int Checking(Massive a, const int b1, const int b2) {
	//c.c. doesn't match/
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
	
	//checking original on b1//
	if (b1check(a, b1)==0) {
		return 0;
	}

	return 1;
}
//PRINTING
void Printing(Massive a, const int b1, const int b2) {
	if (Checking(a, b1, b2) == 0) {
		cout << "bad input";
		return;
	}
	else {
		if (b1 == b2) {
			for (int i = 0; i < a.num; ++i, ++a.adr) {
				cout << *a.adr;
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
	char chislo[13] = {};
	g >> b1 >> b2;
	Massive original;
	original.num = 0;
	while (original.num < 13) {
		g >> chislo[original.num];
		++original.num;
	}
	g.close();

	original.adr = chislo;
	original.num = Number(chislo);
	original.PointLoc = SearchPoint(original);
	Printing(original, b1, b2);
	//system("pause");
	return 0;
}
