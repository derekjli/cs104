#include <stack>
#include <fstream>
#include <sstream>

//main execution function
int main(int argc, char** argv) {
    // empty stack
        std::stack<char> directions;

        // open the file
        std::ifstream input(argv[1]);

        // read line by line
        std::string line;

        //loop through input line by line
        while(std::getline(input, line))
        {
            // read input as character
            std::istringstream directions_stream(line);
            char direction;

            // get the directions
            directions_stream >> direction;
            
            // push directions onto the stack
            if (directions.empty())
            {
                directions.push(direction);
                continue;
            }

            // check if the direction is N or S
            // pop off the stack if input is N or S and top of stack is N or S
            if (direction == 'N' || direction == 'S')
            {
                if (direction == 'S' && directions.top() == 'N')
                {
                    directions.pop();
                }
                else if (direction == 'N' && directions.top() == 'S')
                {
                    directions.pop();
                }
                // if no popped direction, push
                else
                {
                    directions.push(direction);
                }
            }

            // check if the directions is E or W
            // pop off the stack if input is N or S and top of stack is N or S
            else if (direction == 'E' || direction == 'W')
            {
                if (direction == 'E' && directions.top() == 'W')
                {
                    directions.pop();
                }
                else if (direction == 'W' && directions.top() == 'E')
                {
                    directions.pop();
                }
                else
                {
                    directions.push(direction);
                }
            }

        }
}