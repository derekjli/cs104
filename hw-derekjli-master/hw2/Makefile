all: basic_interpreter

basic_interpreter: main.cpp arithmetic.o command.o interpreter.o 
	g++ -g -Wall -std=c++11 main.cpp arithmetic.o command.o interpreter.o -o basic_interpreter

arithmetic.o: interpreter/arithmetic.cpp
	g++ -g -Wall -c -std=c++11 interpreter/arithmetic.h interpreter/arithmetic.cpp

command.o: interpreter/command.cpp 
	g++ -g -Wall -c -std=c++11 interpreter/command.h interpreter/command.cpp

interpreter.o: interpreter/interpreter.cpp   
	g++ -g -Wall -c -std=c++11 interpreter/interpreter.h interpreter/interpreter.cpp