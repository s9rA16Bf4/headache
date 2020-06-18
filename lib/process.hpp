#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class process{

public:
  bool compare(std::string, std::string);
  std::vector<std::string> readGuts(std::string);
  std::vector<std::string> split(std::string, std::string);
};

#endif
