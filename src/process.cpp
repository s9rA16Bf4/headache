#include "process.hpp"

std::vector<std::string> process::readGuts(std::string filePath){
  std::ifstream openFile(filePath);
  std::vector<std::string> toReturn;

  if (openFile.is_open()){
    std::string line = "";

    while(std::getline(openFile,line)){
      if (line[0] != '%'){ toReturn.push_back(line); } // Ignoring comments
    }
    openFile.close();
  }else{ std::cerr << "Failed to open file [" << filePath << "]"<< std::endl; }
  return toReturn;
}

std::vector<std::string> process::split(std::string line, std::string delimiter){
  std::vector<std::string> toReturn;
  std::size_t delimPrev = 0, delimPos = 0;

  while(line.size() > delimPrev && line.size() > delimPos){
    delimPos = line.find(delimiter, delimPrev);
    if (delimPos == std::string::npos){ delimPos = line.size(); }
    std::string subStr = line.substr(delimPrev, delimPos-delimPrev);
    if (!subStr.empty()){ toReturn.push_back(subStr); }
    delimPrev = delimPos+delimiter.size();
  }

  return toReturn;
}

bool process::compare(std::string a, std::string b){
  bool toReturn = false;

  if (a.size() == b.size()){
    for (unsigned int i = 0; i < a.size(); i++){
      if (a[i] == b[i]){ toReturn = true; }
      else{ toReturn = false; break; }
    }
  }

  return toReturn;
}
