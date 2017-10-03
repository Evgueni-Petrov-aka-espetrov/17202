#include "Ratio.h"

// -------------
// Constructors:

Ratio::Ratio() : num(1), den(1) {  }
Ratio::Ratio(long long numerator, long long denominator) : num(numerator), den(denominator) { Refresh(); }

// -------------
// Private Methods:

void Ratio::Refresh() {
	long long gcd = GCD(num, den);
	
	if (gcd) {
		num /= gcd;
		den /= gcd;
	}

	if (den < 0) {
		num = -num;
		den = -den;
	}
}

long long Ratio::GCD(long long a, long long b) {
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

int Ratio::CharToInt(char digit) {
	digit = toupper(digit);

	if ((digit >= '0') && (digit <= '9')) { return digit - '0'; }
	if ((digit >= 'A') && (digit <= 'F')) { return (digit - 'A') + 10; }

	throw invalid_argument(R"("digit" parameter is invalid)");
}

char Ratio::IntToChar(int digit, bool upperRes) {
	if ((digit < 0) || (digit >= 16)) { throw invalid_argument(R"("digit" parameter is invalid)"); }

	if (digit < 10) { return '0' + digit; }
	else { return ((upperRes) ? 'A' : 'a') + (digit - 10); }
}

// -------------
// Public Methods:

string Ratio::ToString(int radix) {
	if ((radix < 2) || (radix > 16)) { throw invalid_argument("Bad radix"); }
	if (this->den == 0) { throw invalid_argument("Denumenator is zero"); }

	Ratio r = *this;
	long long integer_part;
	string result, int_str;

	if (r.num < 0) {
		result += '-';
		r.num = -r.num;
	}

	integer_part = r.num / r.den;
	r.num %= r.den;

	while (integer_part) {
		int_str = IntToChar(integer_part % radix) + int_str;
		integer_part /= radix;
	}
	result += (int_str.length() ? int_str : "0"s);

	if (r.num) {
		result += '.';

		for (int i = 0; i < 12; ++i) {
			r.num *= radix;
			result += IntToChar(r.num / r.den);
			r.num %= r.den;

			if (!r.num) { break; }
		}
	}

	return result;
}

bool Ratio::StrIsNumber(const string &str, int radix) {
	if ((radix < 2) || (radix > 16)) { throw invalid_argument("Bad radix"); }

	string char_set;
	regex r;

	char_set = "0-"s + IntToChar(min(radix - 1, 9));
	if (radix > 10) {
		char_set += "a-"s + IntToChar(radix - 1, false);
		char_set += "A-"s + IntToChar(radix - 1, true);
	}

	r = R"(-?(?:[)"s + char_set + R"(]+)(?:\.[)"s + char_set + R"(]+)?)"s;

	return regex_match(str, r);
}

Ratio Ratio::StrToRatio(const string &str, int radix) {
	if (!StrIsNumber(str, radix)) { throw invalid_argument("String is not a number"); }

	Ratio result(0, 1);

	auto it = str.cbegin();
	auto end = str.cend();
	
	if (*it == '-') {
		result.den = -result.den;
		++it;
	}

	for (; it != end; ++it) {
		if (*it == '.') { break; }

		result.num *= radix;
		result.num += CharToInt(*it);
	}

	if (it != end) {
		for (++it; it != end; ++it) {
			result.num *= radix;
			result.den *= radix;

			result.num += CharToInt(*it);
		}
	}

	result.Refresh();
	return result;
}