all: basic_interpreter

basic_interpreter: main.cpp arithmetic.o command.o interpreter.o 
    g++ -g -Wall -c -std=c++11 main.cpp arithmetic.o command.o interpreter.o -o basic_interpreter

arithmetic.o: arithmetic.h arithmetic.cpp
    g++ -g -Wall -c -std=c++11 arithmetic.h arithmetic.cpp -o arithmetic.o

command.o: command.h command.cpp 
    g++ -g -Wall -c -std=c++11 command.h command.cpp -o command.o

interpreter.o: interpreter.h interpreter.cpp   
    g++ -g -Wall -c -std=c++11 interpreter.h interpreter.cpp -o interpreter.o