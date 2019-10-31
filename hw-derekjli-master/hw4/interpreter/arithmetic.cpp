#include "arithmetic.h"
#include <sstream>

NumericExpression::~NumericExpression() {}

NumberLiteralExpression::NumberLiteralExpression(int value) : value(value) {}
NumberLiteralExpression::~NumberLiteralExpression() {}

std::string NumberLiteralExpression::format() const {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

// Don't forget format later :P
VariableExpression::VariableExpression(std::string varName) : varName(varName) {}
VariableExpression::~VariableExpression() {}
std::string VariableExpression::format() const {
	return varName;
}

//Addition Expression
AdditionExpression::AdditionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
AdditionExpression::~AdditionExpression() {
	delete this->left;
	delete this->right;
}
std::string AdditionExpression::format() const {
	return "(" + this->left->format() + " + " + this->right->format() + ")";
}

SubtractionExpression::SubtractionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
SubtractionExpression::~SubtractionExpression() {
	delete this->left;
	delete this->right;
}
std::string SubtractionExpression::format() const {
	return "(" + this->left->format() + " - " + this->right->format() + ")";
}

DivisionExpression::DivisionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
DivisionExpression::~DivisionExpression() {
	delete this->left;
	delete this->right;
}
std::string DivisionExpression::format() const {
	return "(" + this->left->format() + " / " + this->right->format() + ")";
}


MultiplicationExpression::MultiplicationExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
MultiplicationExpression::~MultiplicationExpression() {
	delete this->left;
	delete this->right;
}
std::string MultiplicationExpression::format() const {
	return "(" + this->left->format() + " * " + this->right->format() + ")";
}


ArraySubscriptExpression::ArraySubscriptExpression(std::string arrayName, NumericExpression* index) : arrayName(arrayName), index(index) {}
ArraySubscriptExpression::~ArraySubscriptExpression() {
	delete this->index;
}
std::string ArraySubscriptExpression::format() const {
	return arrayName + "[" + index->format() + "]";
}


BooleanExpression::~BooleanExpression() {}

EqualToExpression::EqualToExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
EqualToExpression::~EqualToExpression() {
	delete this->left;
	delete this->right;
}
std::string EqualToExpression::format() const {
	return left->format() + " = " + right->format();
}

LessThanExpression::LessThanExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
LessThanExpression::~LessThanExpression() {
	delete this->left;
	delete this->right;
}
std::string LessThanExpression::format() const {
	return left->format() + " < " + right->format();
}

GreaterThanExpression::GreaterThanExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {}
GreaterThanExpression::~GreaterThanExpression() {
	delete this->left;
	delete this->right;
}
std::string GreaterThanExpression::format() const {
	return left->format() + " > " + right->format();
}
