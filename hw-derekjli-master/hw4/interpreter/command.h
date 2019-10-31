#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include "arithmetic.h"

class Command {
public:
	Command();
	virtual ~Command();
	virtual std::string format() const = 0;
	 // exec() takes in env to add and get context
	int void exec(Interpreter& env) const = 0; 
};

class IfStatement : public Command {
public:
	IfStatement(BooleanExpression* condition, int destinationLineNumber);
	~IfStatement();
	virtual std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;

private:
	BooleanExpression* condition;
	int destinationLineNumber;
};

class GotoStatement : public Command {
public:
	GotoStatement(int destinationLineNumber);
	~GotoStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;

private:
	int destinationLineNumber;
};

class PrintStatement : public Command {
public:
	PrintStatement(NumericExpression* value);
	~PrintStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;

private:
	NumericExpression* value;
};

class LetStatement : public Command {
public:
	LetStatement(std::string variableName, NumericExpression* assignedValue);
	~LetStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;
private:
	std::string variableName;
	NumericExpression* assignedValue;
}

class ArrayLetStatement : public Command {
public:
	ArrayLetStatement(std::string arrayName, NumericExpression* assignedValue, NumericExpression* index);
	~ArrayLetStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;
private:
	std::string arrayName;
	NumericExpression* assignedValue;
	NumericExpression* index;
};

class GoSubStatement : public Command {
public:
	GoSubStatement(int destinationLineNumber);
	~GoSubStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;
private:
	int destinationLineNumber;
};

class ReturnStatement : public Command {
public:
	ReturnStatement();
	~ReturnStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;
};

class EndStatement : public Command {
public:
	EndStatement();
	~EndStatement();
	std::string format() const;
	virtual void exec(Interpreter& env, int linenum) const;
};

#endif
