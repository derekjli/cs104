#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>

class NumericExpression {
public:
	virtual ~NumericExpression();
	virtual std::string format() const = 0;
};

//normal number
class NumberLiteralExpression : public NumericExpression {
public:
	NumberLiteralExpression(int value);
	~NumberLiteralExpression();

	std::string format() const;
private:
	int value;
};

//addition expression
class AdditionExpression : public NumericExpression {
public:
	AdditionExpression(NumericExpression* left, NumericExpression* right);
	~AdditionExpression();

	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

class SubtractionExpression : public NumericExpression {
public:
	SubtractionExpression(NumericExpression* left, NumericExpression* right);
	~SubtractionExpression();
	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

class MultiplicationExpression : public NumericExpression {
public:
	MultiplicationExpression(NumericExpression* left, NumericExpression* right);
	~MultiplicationExpression();

	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

class DivisionExpression : public NumericExpression {
public:
	DivisionExpression(NumericExpression* left, NumericExpression* right);
	~DivisionExpression();

	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

class VariableExpression : public NumericExpression {
public:
	VariableExpression(std::string varName);
	~VariableExpression();
	std::string format() const;
private:
	std::string varName;
};

//
class ArraySubscriptExpression : public NumericExpression {
public:
	ArraySubscriptExpression(std::string arrayName, NumericExpression* index);
	~ArraySubscriptExpression();
	std::string format() const;

private:
	std::string arrayName;
	NumericExpression* index;
};

class BooleanExpression {
public:
	virtual ~BooleanExpression();
	virtual std::string format() const = 0;

};

class EqualToExpression : public BooleanExpression {
public:
	EqualToExpression(NumericExpression* left, NumericExpression* right);
	~EqualToExpression();
	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

class LessThanExpression : public BooleanExpression {
public:
	LessThanExpression(NumericExpression* left, NumericExpression* right);
	~LessThanExpression();
	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

class GreaterThanExpression : public BooleanExpression {
public:
	GreaterThanExpression(NumericExpression* left, NumericExpression* right);
	~GreaterThanExpression();
	std::string format() const;

private:
	NumericExpression* left;
	NumericExpression* right;
};

#endif