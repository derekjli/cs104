#include "interpreter.h"
#include <cctype>

//parse input
Interpreter::Interpreter(std::istream& in) {
	this->parse(in);
}

//destruct and deallocate the commands
Interpreter::~Interpreter() {
	for (auto command : m_Commands) {
		delete command.second; //deallocate dynamically allocated Command
	}
}

//execute the commands
void Interpreter::execute(){

	std::map<int,Command* >::iterator it = m_Commands->begin();	
	//stores line number program is currently executing
	int current_line_number = it->first;
	
	// execute till END statement
	while(current_line_number != -1){
		// store next line number
		int next_line_number = (++it)->first;
		// exec sets new current_line_number based on command 
		current_line_number = m_Commands[current_line_number]->exec(*this, next_line_number);
	}
}

//parse the input
void Interpreter::parse(std::istream& in) {
	std::string line;
	while (std::getline(in, line)) {

		//split the line into syntax nodes
        //convert string into vector of syntax nodes
		std::vector<std::string> pieces = split_line(line);

		int line_number = string_to_int(pieces[0]); //get line number
		pieces.erase(pieces.begin()); //erase line number

		//record line numbers and commands
		Command* cmd = parse_command(pieces);

		// Why would you write std::pair here? You can just write { key, value }
		m_Commands.insert({ line_number, cmd });
	}
}

//pretty print every line
void Interpreter::write(std::ostream& out) {
	for (auto command : m_Commands) {
		out << command.first << ' ' << command.second->format() << std::endl;
	}
}

//convert string into int
//used in places such as taking in the string element as an int 
//to construct a numexpression
int Interpreter::string_to_int(std::string const& str) {
	std::stringstream ss;
	ss << str;
	int n = 0;
	ss >> n;
	return n;
}

//iterate through each line of text and 
//and return its sensible syntax nodes
std::vector<std::string> Interpreter::split_line(std::string const& line) {
	std::vector<std::string> pieces;
	int offset = 0;

    //loop through the line
	while (offset < line.length()) {

        //get rid of spaces
		if (std::isspace(line[offset])) {
			// Just skip
			++offset;
		}

        //get the identifier name
        //if it's an identifier, consume until end of identifier, then add it
		else if (std::isalpha(line[offset])) {
			std::string variableName;
				while (std::isalpha(line[offset])) {
					variableName += line[offset];
						offset++;
				}
			pieces.push_back(variableName);
		}
        //if number, do the same as with identifier
		else if (std::isdigit(line[offset])) {
			std::string number;
			while (std::isdigit(line[offset])) {
				number += line[offset];
				offset++;
			}
			pieces.push_back(number);
		}
        //if binary operator, brackets, or parentheses
        else {
			std::string piece;
			piece += line[offset];
			++offset;
			pieces.push_back(piece);
		}
	}
	return pieces;
}

//parse command and return command object
//traversing methodology: consume everything you have read from source
//by erasing what you have read in the front of the vector
Command* Interpreter::parse_command(std::vector<std::string>& source) {
	
    // get command and get first param to command
    // get rid of command
	std::string command = source[0];
	source.erase(source.begin());

    //check commands

	if (command == "PRINT") {
        //recursively parse numexp
		NumericExpression* numexp = parse_numeric_expression(source);
		return new PrintStatement(numexp);
	}
	else if (command == "GOTO") {
        //get line number
		int linenum = string_to_int(source[0]);
		source.erase(source.begin());
		return new GotoStatement(linenum);
	}
	else if (command == "GOSUB") {
		// same as GOTO
		int linenum = string_to_int(source[0]);
		source.erase(source.begin());//consume linenum
		return new GoSubStatement(linenum);
	}
	else if (command == "LET") {
        //store var name
		std::string variableName = source[0];
		source.erase(source.begin());
        
		// check if array or variable literal
		if (source[0] == "[") {
			// Here we have the let array command
			source.erase(source.begin()); //consume '['
			NumericExpression* numexp = parse_numeric_expression(source);
			source.erase(source.begin()); //consume ']'

			NumericExpression* assigned_exp = parse_numeric_expression(source); //parse numeric
			return new ArrayLetStatement(variableName, assigned_exp, numexp);
		}
		else {
			// Simple let, not an array
			NumericExpression* numliteral = parse_numeric_expression(source);
			return new LetStatement(variableName, numliteral);
		}
	}
	else if (command == "IF") {
		BooleanExpression* boolexp = parse_boolean_expression(source);
		source.erase(source.begin()); // consume THEN
		int linenum = string_to_int(source[0]);
		source.erase(source.begin()); // consume linenume
		return new IfStatement(boolexp, linenum);
	}
	else if (command == "RETURN") {
		// Nothing to parse
		return new ReturnStatement();   
	}
	else if (command == "END") {
		// Nothing to parse
		return new EndStatement();
	}
}

//parse numeric expression recursively
NumericExpression* Interpreter::parse_numeric_expression(std::vector<std::string>& source) {
	/*
	We have 4 possibilities. A numeric expr. can be:
	 - A number literal (with a negative sign too!)
	 - A variable (identifier)
	 - An array subscript
	 - A binary operation
	*/
	
	//if positive number
	if (std::isdigit(source[0][0])) {
        //check if digit, if so, return int version
		NumericExpression* number = new NumberLiteralExpression(string_to_int(source[0]));
		source.erase(source.begin()); //consume number
		return number;
	}
	//if negative number
	else if (source[0] == "-") {
		NumericExpression* number = new NumberLiteralExpression(-string_to_int(source[1]));//   this will actually work? just adding "-"
		source.erase(source.begin()); //consume '-'
		source.erase(source.begin()); //consume number
		return number;
	}
	//if variable, check if simple var or array
	else if (std::isalpha(source[0][0])) {
		std::string varName = source[0]; //store var name
		source.erase(source.begin());
		// check if array by checking if [ follows
		if (source[0] == "[") {
			source.erase(source.begin()); //consume [
			NumericExpression* index = parse_numeric_expression(source); //parse index
			source.erase(source.begin()); //consume ]
			NumericExpression* arrayValue = new ArraySubscriptExpression(varName, index); //create new arrayvalue
			return arrayValue;
		}
        //plain variable
		else {
			return new VariableExpression(varName);
		}
	}
    //if arithmetic operation, as indicated by (, parse left and right
	else if (source[0] == "(") {
		source.erase(source.begin()); //consume '('
        // recursively parses the left-hand side
		NumericExpression* left = parse_numeric_expression(source);
		//read in the binary ops
		std::string binop = source[0];
		source.erase(source.begin()); //consume arithmetic binary operation
		NumericExpression* right = parse_numeric_expression(source); // rescursively parses the right-hand side
		source.erase(source.begin()); //consume ')'

        //determine expression based on arithmetic binary operation
		if (binop == "+")
			return new AdditionExpression(left, right);
		else if (binop == "-")
			return new SubtractionExpression(left, right);
		else if (binop == "*")
			return new MultiplicationExpression(left, right);
		else
			return new DivisionExpression(left, right);
            
		//LOGIC:
        // the logic is based on the fact that you will have a numeric expression inside of a numeric expression.
        // so whenver there is the possiblity of a numeric expression, simply parse that numeric expression, and store it
        // as a member of another numeric expression
        // therefore, we can create a recursive method that will do this.
	}
	
    // For a base we can return NULL. That will tell us if we have an error
	return NULL;
}

//parse the boolean expression
// parse a numeric, consume an operator, parse another numeric
// construct the boolean expression based on operator
BooleanExpression* Interpreter::parse_boolean_expression(std::vector<std::string>& source) {
	// 3 operators: '=', '>' and '<'
    // parse the left-hand side
	NumericExpression* left = parse_numeric_expression(source);
	// read in and consume an operator
	std::string binop = source[0];
	source.erase(source.begin());
	// now we parse the right-hand side numeric
	NumericExpression* right = parse_numeric_expression(source); // parse the right-hand side
	//create boolean expression
    if (binop == "=")
		return new EqualToExpression(left, right);
	else if (binop == "<")
		return new LessThanExpression(left, right);
	else
		return new GreaterThanExpression(left, right);
}
