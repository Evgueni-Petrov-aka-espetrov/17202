#include <cmath>
#include <string>
#include <regex>
#include "std_extended.h"

using namespace std;
using namespace std_extended;

class Ratio {
private:
	long long num, den;

	void Refresh();
public:
	Ratio();
	Ratio(long long numerator, long long denominator);

	string ToString(int radix = 10, bool upperRes = true, size_t precision = 12u) const;

	static bool StrIsNumber(const string &str, int radix = 10);
	static Ratio StrToRatio(const string &str, int radix = 10);
};
