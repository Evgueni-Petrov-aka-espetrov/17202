#pragma once
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>

using namespace std;

// Map which consists of binary operators (symbols and behaviors) with same priority
typedef map<string, function<int(int, int)>> operatorMap;

class MathExpr {
private:
	list<operatorMap> priorityOperators;
	set<string> operatorSet;
	string expression;

	bool isCalculated;
	bool isCorrect;
	int result;
	
	int CheckExpression() const;
	
	bool Add(const string &op, function<int(int, int)> behavior, list<operatorMap>::iterator pos);
	void CalculateExpression();

	pair<list<operatorMap>::iterator, operatorMap::iterator> FindOperator(const string &op);
	static int NumberTokenToInt(string::const_iterator beg, string::const_iterator end);
	static bool isOperatorChar(char ch);

	struct Triada {
		pair<list<operatorMap>::const_iterator, operatorMap::const_iterator> op;
		int level;
		list<int>::iterator left, right;
	};

public:
	MathExpr();
	explicit MathExpr(const MathExpr &expr);
	explicit MathExpr(MathExpr &&expr) noexcept;
	explicit MathExpr(const string &expr);
	explicit MathExpr(string &&expr);

	MathExpr& operator= (const MathExpr &rhs);
	MathExpr& operator= (MathExpr &&rhs) noexcept;
	MathExpr& operator= (const string &rhs);
	MathExpr& operator= (string &&rhs);

	bool AddOperatorWithSamePriority(const string &op, function<int(int, int)> behavior, const string &relativeOperator);
	bool AddOperatorWithHigherPriority(const string &op, function<int(int, int)> behavior, const string &relativeOperator);
	bool AddOperatorWithLowerPriority(const string &op, function<int(int, int)> behavior, const string &relativeOperator);
	bool AddOperatorWithHighestPriority(const string &op, function<int(int, int)> behavior);
	bool AddOperatorWithLowestPriority(const string &op, function<int(int, int)> behavior);

	pair<list<operatorMap>::const_iterator, operatorMap::const_iterator> FindOperator(const string &op) const;

	bool ExistsOperator(const string &op) const;
	string GetExpressionString() const;
	pair<bool, int> GetExpressionResult();
};
