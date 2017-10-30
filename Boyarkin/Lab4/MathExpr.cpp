#include "MathExpr.h"

MathExpr::MathExpr() :
	expression(),
	isCalculated(true),
	isCorrect(false) {  }
MathExpr::MathExpr(const MathExpr &expr) :
	priorityOperators(expr.priorityOperators),
	operatorSet(expr.operatorSet),
	expression(expr.expression),
	isCalculated(expr.isCalculated),
	isCorrect(expr.isCorrect),
	result(expr.result) {  }
MathExpr::MathExpr(MathExpr &&expr) noexcept :
	priorityOperators(move(expr.priorityOperators)),
	operatorSet(move(expr.operatorSet)),
	expression(move(expr.expression)),
	isCalculated(expr.isCalculated),
	isCorrect(expr.isCorrect),
	result(expr.result)
{
	expr.isCalculated = false;
}
MathExpr::MathExpr(const string &expr) :
	expression(expr),
	isCalculated(false) {  }
MathExpr::MathExpr(string &&expr) :
	expression(std::move(expr)),
	isCalculated(false) {  }

MathExpr& MathExpr::operator= (const MathExpr &rhs) {
	if (this == &rhs) { return *this; }

	this->priorityOperators = rhs.priorityOperators;
	this->operatorSet = rhs.operatorSet;
	this->expression = rhs.expression;
	this->isCalculated = rhs.isCalculated;
	this->isCorrect = rhs.isCorrect;
	this->result = rhs.result;

	return *this;
}
MathExpr& MathExpr::operator= (MathExpr &&rhs) noexcept {
	if (this == &rhs) { return *this; }

	this->priorityOperators = move(rhs.priorityOperators);
	this->operatorSet = move(rhs.operatorSet);
	this->expression = move(rhs.expression);
	this->isCalculated = rhs.isCalculated;
	this->isCorrect = rhs.isCorrect;
	this->result = rhs.result;

	rhs.isCalculated = false;

	return *this;
}
MathExpr& MathExpr::operator= (const string &rhs) {
	this->expression = rhs;
	this->isCalculated = false;

	return *this;
}
MathExpr& MathExpr::operator= (string &&rhs) {
	this->expression = move(rhs);
	this->isCalculated = false;

	return *this;
}

int MathExpr::CheckBrackets() const {
	int level = 0;
	int max_level = 0;

	for (auto it = expression.cbegin(); it != expression.cend(); ++it) {
		if (*it == '(') {
			++level;
			if (level > max_level) { max_level = level; }


			if (it != expression.cbegin()) {
				if ((*(it - 1) == ')') || isdigit(*(it - 1))) { return -1; }
			}

			if ((it + 1) != expression.cend()) {
				if ((*(it + 1) != '(') && !isdigit(*(it + 1))) { return -1; }
			}
		}
		else if (*it == ')') {
			--level;

			if (it != expression.cbegin()) {
				if ((*(it - 1) != ')') && !isdigit(*(it - 1))) { return -1; }
			}

			if ((it + 1) != expression.cend()) {
				if ((*(it + 1) == '(') || isdigit(*(it + 1))) { return -1; }
			}
		}

		if (level < 0) { return -1; }
	}
	if (level != 0) { return -1; }

	return max_level;
}

bool MathExpr::Add(char op, function<int(int, int)> behavior, list<operatorMap>::iterator pos) {
	if (isdigit(op) || (op == '(') || (op == ')')) { return false; }

	isCalculated = false;

	operatorSet.insert(op);
	pos->insert(make_pair(op, behavior));

	return true;
}

void MathExpr::CalculateExpression() {
	if (isCalculated) { return; }
	isCalculated = true;
	isCorrect = false;

	int max_level = CheckBrackets();
	if (max_level == -1) { return; }

	Triada t;
	list<int> numbers;
	list<Triada> operators;
	vector<queue<list<Triada>::iterator>> prior;

	int level;
	string::const_iterator numberIter;
	list<int>::iterator numPtr;

	// Read list of numbers:
	for (auto it = expression.cbegin(); it != expression.cend(); ++it) {

		if (isdigit(*it)) {

			numberIter = it;
			while (numberIter != expression.cend() && isdigit(*numberIter)) { ++numberIter; }
			numbers.push_back(NumberTokenToInt(it, numberIter));

			it = numberIter;
			--it;
		}
	}

	// Read list of operators:
	numPtr = numbers.begin();
	level = 0;
	for (auto it = expression.cbegin(); it != expression.cend(); ++it) {
		if (*it == '(') { ++level; continue; }
		if (*it == ')') { --level; continue; }
		if (isdigit(*it)) { continue; }

		if (!ExistsOperator(*it)) { return; }

		t.op = *it;
		t.level = level;

		if (numPtr == numbers.end()) { return; }
		t.left = numPtr;

		++numPtr;

		if (numPtr == numbers.end()) { return; }
		t.right = numPtr;

		operators.push_back(t);
	}
	if ((operators.size() + 1) != numbers.size()) { return; }

	// Get order of calculating operators:
	prior.resize(max_level + 1);
	for (auto listIter = priorityOperators.cbegin(); listIter != priorityOperators.cend(); ++listIter) {

		for (auto triadaIter = operators.begin(); triadaIter != operators.end(); ++triadaIter) {
			for (auto mapIter = listIter->cbegin(); mapIter != listIter->cend(); ++mapIter) {

				if (triadaIter->op == mapIter->first) {
					prior[triadaIter->level].push(triadaIter);
				}
			}
		}
	}

	// Calculate:
	for (auto pIter = prior.rbegin(); pIter != prior.rend(); ++pIter) {
		while (!pIter->empty()) {

			auto item = pIter->front();
			auto item_right = item;
			auto& func = FindOperator(item->op).second->second;

			++item_right;

			*(item->left) = func(*(item->left), *(item->right));
			if (item_right != operators.end()) {
				item_right->left = item->left;
			}

			operators.erase(item);
			pIter->pop();
		}
	}

	result = *numbers.begin();
	isCorrect = true;
}

bool MathExpr::AddOperatorWithSamePriority(char op, function<int(int, int)> behavior, char relativeOperator) {
	if (ExistsOperator(op)) { return false; }
	if (!ExistsOperator(relativeOperator)) { return false; }

	auto listIter = FindOperator(relativeOperator).first;
	return Add(op, behavior, listIter);
}

bool MathExpr::AddOperatorWithHigherPriority(char op, function<int(int, int)> behavior, char relativeOperator) {
	if (ExistsOperator(op)) { return false; }
	if (!ExistsOperator(relativeOperator)) { return false; }

	auto listIter = FindOperator(relativeOperator).first;
	listIter = priorityOperators.insert(listIter, operatorMap());
	return Add(op, behavior, listIter);
}

bool MathExpr::AddOperatorWithLowerPriority(char op, function<int(int, int)> behavior, char relativeOperator) {
	if (ExistsOperator(op)) { return false; }
	if (!ExistsOperator(relativeOperator)) { return false; }

	auto listIter = FindOperator(relativeOperator).first;
	listIter = priorityOperators.insert(++listIter, operatorMap());
	return Add(op, behavior, listIter);
}

bool MathExpr::AddOperatorWithHighestPriority(char op, function<int(int, int)> behavior) {
	if (ExistsOperator(op)) { return false; }

	priorityOperators.push_front(operatorMap());
	return Add(op, behavior, priorityOperators.begin());
}

bool MathExpr::AddOperatorWithLowestPriority(char op, function<int(int, int)> behavior) {
	if (ExistsOperator(op)) { return false; }

	priorityOperators.push_back(operatorMap());
	return Add(op, behavior, --priorityOperators.end());
}

pair<list<operatorMap>::const_iterator, operatorMap::const_iterator> MathExpr::FindOperator(char op) const {
	if (!ExistsOperator(op)) {
		return make_pair(priorityOperators.cend(), operatorMap::const_iterator());
	}

	operatorMap::const_iterator ptr;

	for (auto it = priorityOperators.cbegin(); it != priorityOperators.cend(); ++it) {
		ptr = it->find(op);

		if (ptr != it->cend()) { return make_pair(it, ptr); }
	}

	return make_pair(priorityOperators.cend(), operatorMap::const_iterator());
}

pair<list<operatorMap>::iterator, operatorMap::iterator> MathExpr::FindOperator(char op) {
	if (!ExistsOperator(op)) {
		return make_pair(priorityOperators.end(), operatorMap::iterator());
	}

	operatorMap::iterator ptr;

	for (auto it = priorityOperators.begin(); it != priorityOperators.end(); ++it) {
		ptr = it->find(op);

		if (ptr != it->end()) { return make_pair(it, ptr); }
	}

	return make_pair(priorityOperators.end(), operatorMap::iterator());
}

bool MathExpr::ExistsOperator(char op) const {
	return operatorSet.find(op) != operatorSet.cend();
}

string MathExpr::GetExpressionString() const {
	return this->expression;
}

pair<bool, int> MathExpr::GetExpressionResult() {
	CalculateExpression();

	return make_pair(isCorrect, result);
}

int MathExpr::NumberTokenToInt(string::const_iterator beg, string::const_iterator end) {
	int result = 0;

	while (beg != end) {
		result = (result * 10) + (*beg - '0');
		++beg;
	}

	return result;
}
