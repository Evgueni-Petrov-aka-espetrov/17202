#include <fstream>
#include <string>

#include "Ratio.h"

using namespace std;

int main() {
	ifstream in("in.txt");
	ofstream out("out.txt");

	if (in.bad()) {
		out << "Couldn't open file.";
		return 1;
	}

	int b1, b2;
	string str;
	Ratio x;

	in >> b1 >> b2;
	in >> str;

	try {
		x = Ratio::StrToRatio(str, b1);
		out << x.ToString(b2);
	}
	catch (...) { out << "bad input"; } // Something bad happened. We presume it happened because of bad input.

	return 0;
}