#include "Ratio.h"

// -------------
// Constructors:

Ratio::Ratio() : num(0), den(1) {  }
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

// -------------
// Public Methods:

string Ratio::ToString(int radix, bool upperRes, size_t precision) const {
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
		int_str = IntToChar(integer_part % radix, upperRes) + int_str;
		integer_part /= radix;
	}
	//result += (int_str.length() ? int_str : "0"s);
	result += (int_str.length() ? int_str : string("0"));

	if (r.num && (precision != 0)) {
		result += '.';

		for (int i = 0; i < precision; ++i) {
			r.num *= radix;
			result += IntToChar(static_cast<int>(r.num / r.den), upperRes);
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

	char_set = string("0-") + IntToChar(min(radix - 1, 9));
	if (radix > 10) {
		char_set += string("a-") + IntToChar(radix - 1, false);
		char_set += string("A-") + IntToChar(radix - 1, true);
	}

	//r = R"(-?(?:[)"s + char_set + R"(]+)(?:\.[)"s + char_set + R"(]+)?)"s;
	r = string(R"(-?(?:[)") + string(char_set) + string(R"(]+)(?:\.[)") + string(char_set) + string(R"(]+)?)");

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
		while (*(end - 1) == '0') { --end; }

		for (++it; it != end; ++it) {
			result.num *= radix;
			result.den *= radix;

			result.num += CharToInt(*it);
		}
	}

	result.Refresh();
	return result;
}
