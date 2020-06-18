#ifndef ERRORHANDLING_HPP
#define ERRORHANDLING_HPP

inline static void error(std::string errorMessage){
  std::cout << "#### Error ####" << std::endl;
  std::cout << "\n" << errorMessage << std::endl;
  std::cout << "\n###############" << std::endl;
}

#endif
