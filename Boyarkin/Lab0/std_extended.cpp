#include "std_extended.h"

long long std_extended::GCD(long long a, long long b) {
	a = abs(a);
	b = abs(b);

	while (b) {
		a %= b;

		a ^= b;
		b ^= a;
		a ^= b;
	}
	return a;
}

long long std_extended::LCD(long long a, long long b) {
	return (a / GCD(a, b)) * b;
}

int std_extended::CharToInt(char digit) {
	digit = toupper(digit);

	if ((digit >= '0') && (digit <= '9')) { return digit - '0'; }
	if ((digit >= 'A') && (digit <= 'F')) { return (digit - 'A') + 10; }

	throw invalid_argument(R"("digit" parameter is invalid)");
}

char std_extended::IntToChar(int digit, bool upperRes) {
	if ((digit < 0) || (digit >= 16)) { throw invalid_argument(R"("digit" parameter is invalid)"); }

	if (digit < 10) { return '0' + digit; }
	else { return ((upperRes) ? 'A' : 'a') + (digit - 10); }
}