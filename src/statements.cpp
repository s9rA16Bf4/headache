#include "process.hpp"
#include "builtinFunctions.hpp"

void statements(std::vector<std::string> contents){
  process _p;
  built _bt;

  std::vector<std::string> splitGuts;

  for (unsigned int p = 0; p < contents.size(); p++){
    splitGuts = _p.split(contents[p], " "); // Get each content split by space
    for (unsigned int i = 0; i < splitGuts.size(); i++){
      // Great wall of if's
      if (splitGuts[0] != "%"){ // If the line starts with a % then its a comment and shall be ignored
        if (splitGuts[i] == "let" && splitGuts[i+2] == "="){ _bt.call_def_var(splitGuts[i+1], splitGuts[i+3], false); break; }
        if (splitGuts[i] == "const" && splitGuts[i+2] == "="){ _bt.call_def_var(splitGuts[i+1], splitGuts[i+3], true); break; }
        if (splitGuts[i] == "print"){ _bt.call_print(splitGuts); break;}
        if (splitGuts[i] == "def" && splitGuts.size() == 3 && splitGuts[i+2] == "{"){ // Defining a function
          std::vector<std::string> functionGuts = gatherGuts(i, p, 3, splitGuts, contents); // This will gather the function guts
          std::string functionName = splitGuts[i+1];

          _bt.call_def_func(functionName, "string", functionGuts);
          break;
        }
        if (splitGuts[i] == "inc"){
          bool result = false;
          if (splitGuts.size() == 2){ result = _bt.call_inc(contents, splitGuts[i+1], p, "NULL"); }
          else if (splitGuts.size() == 4){ result = _bt.call_inc(contents, splitGuts[i+1], p, splitGuts[3]); }

          if (result) { p--; }  // sub one to "jump a step up"

          break;
        }
        if ((splitGuts[i] == "if" || splitGuts[i] == "while") && splitGuts[splitGuts.size()-1] == "{"){
          std::vector<std::string> guts = gatherGuts(i, p, splitGuts.size()+1, splitGuts, contents); // This will gather the necessary information
          if (splitGuts[0] == "if"){ _bt.call_if_run(splitGuts, guts); }
          else if (splitGuts[0] == "while"){ _bt.call_while_run(splitGuts, guts); }
          break;
        }
        if (splitGuts[i] == "system" && splitGuts[i+1] == "{"){ _bt.call_system(gatherGuts(i,p,2,splitGuts,contents)); break; }
        if (splitGuts[i] == "exit"){ std::exit(EXIT_FAILURE); } // Exits the program
        else{
          variable *A = _bt.findVariable(splitGuts[i]);
          std::string potentialFuncName = splitGuts[i].substr(0, splitGuts[i].size()-2); // This removes the () from the name
          function *B = _bt.findFunction(potentialFuncName);

          if (A && splitGuts[i+1] == "="){
            if (splitGuts.size() == 5){ // Do we have enough for mathematical operations?
              std::string firstVariableOrValue = splitGuts[2], secondVariableOrValue = splitGuts[4];
              if (A->constValue){ error("Can't modify a const, you can only read from it!"); break; }
              else{
                if (splitGuts[3] == "+"){ A->value =_bt.call_add(firstVariableOrValue, secondVariableOrValue); }
                else if (splitGuts[3] == "-"){ A->value = _bt.call_sub(firstVariableOrValue, secondVariableOrValue); }
                else if (splitGuts[3] == "/"){ A->value = _bt.call_div(firstVariableOrValue, secondVariableOrValue); }
                else if (splitGuts[3] == "//"){ A->value = _bt.call_mod(firstVariableOrValue, secondVariableOrValue); }
                else if (splitGuts[3] == "*"){ A->value = _bt.call_mult(firstVariableOrValue, secondVariableOrValue); }
                else if (splitGuts[3] == "**"){ A->value = _bt.call_raise(firstVariableOrValue, secondVariableOrValue); }
                else{ error("Unknown mathematical operator " + splitGuts[3]); }
              }
              break;

            }else{ _bt.call_upd_var(A, splitGuts[i+2]); break; } // Update the variable
          }
          else if (B){ _bt.call_run_func(B); break; } // Call the function
          else{ error("Unknown syntax "+splitGuts[i]+""); }
        }
      }
    }
  }
 }

static inline std::vector<std::string> gatherGuts(unsigned int &i, unsigned int &p, const int index,  std::vector<std::string> splitGuts, std::vector<std::string> contents){
  std::vector<std::string> guts;
  std::string line;
  process _p;
  if (i+index < splitGuts.size()){ i += index; }// jump index spaces forward from i
  else{ // This means that the current line is something like def functionName { which is all information which is irrelevant
    i = 0;
    p++;
    splitGuts = _p.split(contents[p], " ");
  }
  while(splitGuts[i] != "}"){
      line += splitGuts[i] + " " ; // Gather the line
      i++;
      if (i == splitGuts.size()){ // Reset if we are at the end
        i = 0;
        p++;
        guts.push_back(line); // save
        line = ""; // Reset
        splitGuts = _p.split(contents[p], " ");
      }
    }
  return guts;
}
