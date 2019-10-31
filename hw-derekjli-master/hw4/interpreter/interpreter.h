#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <cstddef>
#include <sstream>
#include <string>
#include "arithmetic.h"
#include "command.h"

class Interpreter {
public:
	Interpreter(std::istream& in);
	~Interpreter();

	void write(std::ostream& out);
	void execute();

private:
	void parse(std::istream& in);

	Command* parse_command(std::vector<std::string>& source);
	NumericExpression* parse_numeric_expression(std::vector<std::string>& source);
	BooleanExpression* parse_boolean_expression(std::vector<std::string>& source);

	std::vector<std::string> split_line(std::string const& line);
	int string_to_int(std::string const& str);

	//variable
	std::map <std::string, int> variables;

	// line numbers
	std::stack<int> callback_linenums; 

	// map of line number -> command
	std::map<int, Command*> m_Commands;
};

#endif
