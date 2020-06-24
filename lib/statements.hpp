#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

void statements(std::vector<std::string>); // Prototype declarition
static inline std::vector<std::string> gatherGuts(unsigned int&, std::vector<std::string>); // Prototype declarition

static inline std::vector<std::string> gatherGuts(unsigned int &p, std::vector<std::string> contents){
  std::vector<std::string> guts, localGuts;
  std::string line;
  process _p;
  unsigned int lWing = 1, rWing = 0;

  while(lWing != rWing){
    localGuts = _p.split(contents[++p], " ");
    for (unsigned int i = 0; i < localGuts.size(); i++){
      if (localGuts[i] == "}"){ rWing++; }
      else if (localGuts[i] == "{"){ lWing++; }

      if (i == localGuts.size()-1){
          line += localGuts[i];
        }else{
          line += localGuts[i] + " ";
      }
    }
    guts.push_back(line); // Save the line
    line = ""; // Reset it
  }
  guts.pop_back(); // Removes last object
  return guts;
}

#endif
