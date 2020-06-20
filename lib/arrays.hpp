#ifndef ARRAYS_HPP
#define ARRAYS_HPP

#include <string>
#include <vector>

struct array{
  std::string name;
  std::vector<std::string> gut;

  int size(){ return gut.size(); }
};

#endif
