#include <string>
#include <iostream>

//helper function recursively prints each permutation
void yes(std::string input, std::string maybe) {
  // if buffer is empty
  // all chars have been read
  // is empty print it out
  if(input.empty()) {
    std::cout << maybe << std::endl;
    return;
  }

  //iterate through string
  for(size_t i = 0; i < input.length(); i++) {
    auto aiya = input;
    aiya.erase(i,1); //erase read value
    auto but_maybe = maybe + input[i];
    yes(aiya, but_maybe); //recursively call
  }
}

//main function
void permute(std::string string){
  yes(string, "");
}

int main(){
    permute("USC");
}