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
typedef map<char, function<int(int, int)>> operatorMap;

class MathExpr {
private:
	list<operatorMap> priorityOperators;
	set<char> operatorSet;
	string expression;

	bool isCalculated;
	bool isCorrect;
	int result;
	
	int CheckBrackets() const;
	
	bool Add(char op, function<int(int, int)> behavior, list<operatorMap>::iterator pos);
	void CalculateExpression(); // TODO

	pair<list<operatorMap>::iterator, operatorMap::iterator> FindOperator(char op);
	static int NumberTokenToInt(string::const_iterator beg, string::const_iterator end);

	struct Triada {
		char op;
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

	bool AddOperatorWithSamePriority(char op, function<int(int, int)> behavior, char relativeOperator);
	bool AddOperatorWithHigherPriority(char op, function<int(int, int)> behavior, char relativeOperator);
	bool AddOperatorWithLowerPriority(char op, function<int(int, int)> behavior, char relativeOperator);
	bool AddOperatorWithHighestPriority(char op, function<int(int, int)> behavior);
	bool AddOperatorWithLowestPriority(char op, function<int(int, int)> behavior);

	pair<list<operatorMap>::const_iterator, operatorMap::const_iterator> FindOperator(char op) const;

	bool ExistsOperator(char op) const;
	string GetExpressionString() const;
	pair<bool, int> GetExpressionResult();

	//void PrintPriorities();
};
