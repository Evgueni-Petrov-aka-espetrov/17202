#include <cmath>
#include <string>
#include <regex>

using namespace std;

class Ratio {
private:
	long long num, den;

	void Refresh();
	long long GCD(long long a, long long b);

	static int CharToInt(char digit);
	static char IntToChar(int digit, bool upperRes = true);

public:
	Ratio();
	Ratio(long long numerator, long long denominator);

	string ToString(int radix = 10);

	static bool StrIsNumber(const string &str, int radix = 10);
	static Ratio StrToRatio(const string &str, int radix = 10);
};