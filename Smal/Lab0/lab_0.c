#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//Проверка ввода на правильность
int test_input(const char* full, int length, int not1, int not2) {
	int i = 0;
	char max;
	if (not1 <= 10)
		max = not1 + '0' - 1;
	else {
		max = not1 - 10 + 'a' - 1;
	}

	for (i = 0; i < length; i++) {
		if (*(full + i) > max)
			return 1;
	}
	if (not1 > 16 || not1 < 2)
		return 1;
	if (not2 > 16 || not2 < 2)
		return 1;

	if ((*full == '.') || (*(full + length - 1) == '.'))
		return 1;

	if (strchr(full, '.') != strrchr(full, '.'))
		return 1;
	return 0;
}

//Возведение в степень
double power(int not, int pow) {
	double result = 1;
	int i;
	if (pow >= 0) {
		for (i = 1; i < pow; i++)
			result *= not;
	}
	else {
		pow *= (-1);
		result /= power(not, pow + 1);
	}
	return result;
}

//Перевод числа из not1 в 10-СС
double not1_full_to_dec(const char* full, int length, int not) {
	double summ = 0;
	double chislo = 0;
	int i;
	char* nopt;
	char* nosymb;
	nopt = strchr(full, '.');
	if (nopt == 0)
		nopt = full + length;
	for (i = 0; i < length; i++) {
		if (*(full + i) == '.') {}
		else {
			if (*(full + i) < 'a')
				chislo = *(full + i) - '0';
			else
				chislo = *(full + i) - 'a' + 10;
			nosymb = full + i;
			summ += chislo * power(not, (nopt - nosymb));
		}
	}
	return summ;
}

//Перевод целой части в not2
void dec_cel_to_not2(double chislo, int not2, const FILE* oFile) {
	char local_full[52] = { 0 };
	char* p;

	p = _i64toa(chislo, local_full, not2);
	fprintf(oFile, "%s", p);

}

//Перевод дробной части в not2
void dec_drob_to_not2(double chislo_full, int not2, const FILE* oFile) {
	char drob[14] = { 0 };
	double chislo;
	int i;
	long int cel_chast;
	cel_chast = chislo_full;
	chislo = chislo_full - cel_chast;
	for (i = 0; i < 12; i++)
	{
		if (chislo != 0) {
			chislo *= not2;
			cel_chast = chislo;
			if (cel_chast < 10)
				drob[i] = cel_chast + '0';
			else
				drob[i] = cel_chast + 'a' - 10;
			chislo = chislo - cel_chast;
		}
	}
	fprintf(oFile, ".");
	for (i = 0; i < 12; i++)
		fprintf(oFile, "%c", drob[i]);
}

int main() {
	int i = 0;
	int not1, not2;
	char chislo_full[14];
	FILE *iFile;
	FILE *oFile;
	iFile = fopen("in.txt", "r");
	oFile = fopen("out.txt", "w");
	fscanf(iFile, "%d %d\n", &not1, &not2);

	fscanf(iFile, "%13s", chislo_full);
	fclose(iFile);
	int length = strlen(chislo_full);
	_strlwr(chislo_full);

	if (test_input(chislo_full, length, not1, not2) == 1) {
		fprintf(oFile, "bad input");
		return 0;
	}
	if (not1 == not2) {
		fprintf(oFile, "%s", chislo_full);
		return 0;
	}
	double summ = not1_full_to_dec(chislo_full, length, not1);
	dec_cel_to_not2(summ, not2, oFile);
	if (strchr(chislo_full, '.') != 0)
		dec_drob_to_not2(summ, not2, oFile);
	fclose(oFile);
	return 0;
}