#include <iostream>
#include "process.hpp"
#include "statements.hpp"

int main(int argc, char** argv){
  process _p;
  int toReturn = 1;
  srand(time(NULL)); // For usage when the user want's a random number

  if (argc == 2){ statements(_p.readGuts(argv[1])); toReturn = 0; }
  else{ std::cerr << argv[0] << " <fileToRead>" << std::endl; }
  return toReturn;
}
