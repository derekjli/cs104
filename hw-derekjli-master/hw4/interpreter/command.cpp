#include "command.h"
#include <sstream>
#include <iostream>
​
std::string int_to_str(int n) {
	std::stringstream ss;
	ss << n;
	return ss.str();
}
​
//abstract class for Command
Command::Command() {}
Command::~Command() {}
​
// virtual EXEC()
// executes statement and return next line number given by Interpreter's exec()
​
IfStatement::IfStatement(BooleanExpression* condition, int destinationLineNumber) : condition(condition), destinationLineNumber(destinationLineNumber) {}
IfStatement::~IfStatement() {
	delete condition;
}
std::string IfStatement::format() const {
	return "IF [" + condition->format() + "] THEN <" + int_to_str(destinationLineNumber) + ">";
}
int IfStatement::exec(Interpreter& env, int linenum){
	// go to line number if condition is true
	// else continue
	if (condition->eval(env)){
		return destinationLineNumber;
	}
	return linenum;
}
​
GotoStatement::GotoStatement(int destinationLineNumber) : destinationLineNumber(destinationLineNumber) {}
GotoStatement::~GotoStatement() {}
std::string GotoStatement::format() const {
	return "GOTO <" + int_to_str(destinationLineNumber) + ">";
}
int GotoStatement::exec(Interpreter& env, int linenum){
	return destinationLineNumber;
}
​
PrintStatement::PrintStatement(NumericExpression* value) : value(value) {};
PrintStatement::~PrintStatement() {
	delete value;
};
std::string PrintStatement::format() const {
	return "PRINT " + value->format();
};
int PrintStatement::exec(Interpreter& env, int linenum){
	cout << value->eval(env) << endl;
	return linenum;
}
​
LetStatement::LetStatement(std::string variableName, NumericExpression* assignedValue) : variableName(variableName), assignedValue(assignedValue) {}
LetStatement::~LetStatement() {
	delete assignedValue;
}
std::string LetStatement::format() const {
	return "LET " + variableName + " " + assignedValue->format();
}
// update variables map with new or changed value
int LetStatement::exec(Interpreter& env, int linenum){
	env.variables[variableName][-1] = assignedValue->eval(env);
	return linenum;
}
​
ArrayLetStatement::ArrayLetStatement(std::string arrayName, NumericExpression* assignedValue, NumericExpression* index) : arrayName(arrayName), assignedValue(assignedValue), index(index) {}
ArrayLetStatement::~ArrayLetStatement() {
	delete assignedValue;
	delete index;
}
std::string ArrayLetStatement::format() const {
	return "LET " + arrayName + "[" + index->format() + "]" + " " + assignedValue->format();
}
// update value at array in map variables
int ArrayLetStatement::exec(Interpreter& env, int linenum){
	env.variables[arrayName][index->eval(env)] = assignedValue->eval(env);
	return linenum;
}
​
GoSubStatement::GoSubStatement(int destinationLineNumber) : destinationLineNumber(destinationLineNumber) {}
GoSubStatement::~GoSubStatement() {}
std::string GoSubStatement::format() const {
	return std::string("GOSUB <") + int_to_str(destinationLineNumber) + ">";
}
// add call back line to linenum stack
int GoSubStatement::exec(Interpreter& env, int linenum){
	env.linenums.push(destinationLineNumber);
	return destinationLineNumber;
}
​
ReturnStatement::ReturnStatement() {}
ReturnStatement::~ReturnStatement() {}
std::string ReturnStatement::format() const {
	return "RETURN";
}
// remove call back line that is now being returned
// return that line num
int ReturnStatement::exec(Interpreter& env, int linenum){
	return env.linenums.pop();
}
​
EndStatement::EndStatement() {}
EndStatement::~EndStatement() {}
std::string EndStatement::format()const {
	return "END";
}
// return -1 and exit intepreter execution
int EndStatement::exec(Interpreter& env, int linenum){
	return -1;
}