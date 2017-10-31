#include <fstream>
#include <string>

#include "MathExpr.h"

using namespace std;

int main() {
	ifstream in("in.txt");
	ofstream out("out.txt");

	if (in.fail()) {
		out << "Couldn't open file.";
		return 1;
	}

	MathExpr expr;
	string expression;
	pair<bool, int> result;
	
	expr.AddOperatorWithLowestPriority("+", [](int lhs, int rhs) -> int {
		return lhs + rhs;
	});
	expr.AddOperatorWithSamePriority("-", [](int lhs, int rhs) -> int {
		return lhs - rhs;
	}, "+");
	expr.AddOperatorWithHigherPriority("*", [](int lhs, int rhs) -> int {
		return lhs * rhs;
	}, "+");
	expr.AddOperatorWithSamePriority("/", [](int lhs, int rhs) -> int {
		if (rhs == 0) { throw exception("division by zero"); }
		return lhs / rhs;
	}, "*");
	
	getline(in, expression);
	try {
		expr = expression;
		result = expr.GetExpressionResult();
		
		if (result.first) { out << result.second; }
		else { out << "syntax error"; }
	}
	catch (exception &e) { out << e.what(); }
	catch (...) { out << "Something bad happened"; }

	return 0;
}
