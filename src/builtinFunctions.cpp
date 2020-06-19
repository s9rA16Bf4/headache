#include "builtinFunctions.hpp"

// Public
bool built::call_if_IN(std::string message, std::string word){
  bool toReturn = false;
  for (unsigned int x = 0; x < message.size(); x++){
    for (unsigned int y = 0; y < word.size(); y++){
      if (word[y] == message[x]){ toReturn = true; x++; }
      else{ toReturn = false; break; }
    }
    if (toReturn){ break; }
  }
  return toReturn;
}

void built::call_print(std::vector<std::string> listToPrint){
  for (unsigned int i = 1; i < listToPrint.size(); i++){
    std::string message = listToPrint[i];

    if (message[0] == '$'){ // Varible found!
      message = message.substr(1); // Remove both the wings from 'message'

      variable *A = this->findVariable(message);
      if (!A){ error("Variable "+message+" has not been declared!"); }
      else{
        if (A->value == "nL"){ std::cout << std::endl;}  // Its a newline variable
        else if (A->value == "tL"){ std::cout << "\t"; } // Tab
        else{ std::cout << A->value << " "; }
      }
    }else if (message[0] == '"'){ // Sentence

    }else{ // Its just a message
      if (message != "%"){ std::cout << message << " "; }
      else{ i = listToPrint.size(); }
    }
  }
}

void built::call_def_var(std::string name, std::string value, bool constValue){
  variable *A = this->findVariable(name);

  if (!A){
    if (value[0] == '$'){ // its a variable
      value.erase(0,1); // Removes the dollar sign
      variable *B = this->findVariable(value); // find the value
      if (B){
        if (B->value == "rN"){ value = std::to_string(std::rand() % 10 + 1); }
        else{ value = B->value; }
      }
      else{ error("The variable "+value+" has not been declared!"); return; }
    }
    A = new variable();
    A->name = name;
    A->value = value;
    A->constValue = constValue;
    variables.push_back(A); // Save the variable

  }else{ error("Variable "+name+" has already been declared!"); return; }
}

void built::call_def_func(std::string name, std::string returnType, std::vector<std::string> guts){
  function *A = this->findFunction(name);

  if (!A){
    A = new function();
    A->name = name;
    A->returnType = returnType;
    A->guts = guts;

    functions.push_back(A); // Save the function

  }else{ error("Function "+name+" has already been declared!"); return; }
}

std::vector<std::string> built::call_include(std::string fileName){
  std::fstream openFile(fileName);
  std::vector<std::string> toReturn;

  if (openFile.is_open()){
    std::string line;
    while(std::getline(openFile, line)){ toReturn.push_back(line); }

  }else{ error("The file, "+fileName+", cannot be found in the given path!"); return toReturn; }

  return toReturn;
}

void built::call_upd_var(variable* A, std::string varOrValue){
  if (varOrValue[0] == '$'){ varOrValue.erase(0,1); }
  variable *B = this->findVariable(varOrValue); // Is it a variable or a value?
  if (!B){ A->value = varOrValue; } // its a value
  else{
    if (B->value == "inS"){ std::getline(std::cin, A->value); } // Take an input from the user and save it
    else if (B->value == "rN"){ A->value = std::to_string(std::rand() % 10 + 1); } // Generates a random number between 1 and 10
    else{ A->value = B->value; } // Creates a "copy" of B
  }
}

void built::call_run_func(function *A){
  statements(A->guts);
}

void built::call_if_run(std::vector<std::string> splitGuts, std::vector<std::string> guts){
  std::string valueA = splitGuts[1], valueB = splitGuts[3];
  std::string comparison1 = splitGuts[2];

  if (splitGuts.size() == 5 && this->call_statement_check(valueA, valueB, comparison1)){ statements(guts); }
  else if (splitGuts.size() == 9){
    std::string valueC = splitGuts[5], valueD = splitGuts[7];
    std::string comparison2 = splitGuts[6];

    if (splitGuts[4] == "&" && (this->call_statement_check(valueA, valueB, comparison1) && this->call_statement_check(valueC, valueD, comparison2))){
      statements(guts);
    }else if (splitGuts[4] == "|" && (this->call_statement_check(valueA, valueB, comparison1) || this->call_statement_check(valueC, valueD, comparison2))){
      statements(guts);
    }
  }
}

void built::call_while_run(std::vector<std::string> splitGuts, std::vector<std::string> guts){
  std::string valueA = splitGuts[1], valueB = splitGuts[3];
  std::string comparison1 = splitGuts[2];

  if (splitGuts.size() == 5){
    if (valueA[1] != '$' && valueB[0] != '$'){ error("There most be atleast one variable in a while loop!"); return; }
    else{ while(this->call_statement_check(valueA, valueB, comparison1)){ statements(guts); } } }

  else if (splitGuts.size() == 9){
    std::string valueC = splitGuts[5], valueD = splitGuts[7];
    std::string comparison2 = splitGuts[6];

    if (valueA[1] != '$' && valueB[0] != '$' || valueC[1] != '$' && valueD[0] != '$'){
      error("There most be atleast one variable in a while loop!");
      return;
    }

    if (splitGuts[4] == "&"){
      while(this->call_statement_check(valueA, valueB, comparison1) && this->call_statement_check(valueC, valueD, comparison2)){
        statements(guts);
      }

    }else if (splitGuts[4] == "|"){
      while(this->call_statement_check(valueA, valueB, comparison1) || this->call_statement_check(valueC, valueD, comparison2)){
        statements(guts);
      }
    }
  }
}

variable* built::findVariable(std::string name){
  variable* A = nullptr;

  for (variable* var:variables){
    if (var->name == name){
      A = var;
      break; // If we have found our variable, then there is no point in continuing
    }
  }
  return A;
}

function* built::findFunction(std::string name){
  function* A = nullptr;

  for (function* fun:functions){
    if (fun->name == name){
      A = fun;
      break; // If we have found our variable, then there is no point in continuing
    }
  }
  return A;
}

bool built::call_inc(std::vector<std::string> &guts, std::string fileName, const int index, std::string sectionName){
  std::fstream openFile(fileName);
  bool result = false;
  process _p;

  if (openFile.is_open()){
    std::vector<std::string> newLines, oldLines;
    std::string line;
    while(std::getline(openFile, line)){
      if (sectionName != "NULL" && line.find("def") != std::string::npos){ // We found a function definition
        std::vector<std::string> splitGuts = _p.split(line, " ");
        splitGuts[1] = sectionName+"."+splitGuts[1]; // def <functionName> {
        line = splitGuts[0] + " " + splitGuts[1] + " " + splitGuts[2]; // Create the new line
      }
      newLines.push_back(line); } // save the new lines
    for (int i = index+1; i < guts.size(); i++){ oldLines.push_back(guts[i]); } // save the old lines

    guts.resize(index);
    guts.shrink_to_fit();

    for (int i = 0; i < newLines.size(); i++){ guts.push_back(newLines[i]); } // insert the new lines
    for (int i = 0; i < oldLines.size(); i++){ guts.push_back(oldLines[i]); } // insert the old lines

    result = true;


  }else{ error("File "+fileName+" could not be opened!"); return result; }

  return result;
}

bool built::call_statement_check(std::string &valueA, std::string &valueB, std::string comparison){
  bool toReturn = false;
  std::string tempA = valueA, tempB = valueB; // To remember the past

  if (valueA[0] == '[' && valueA[1] == '$' || valueA[0] == '$'){
    if (valueA[0] == '$'){ valueA.erase(0,1); }
    else{ valueA.erase(0,2); } // This removes the dollarsign and the [ symbol
    // but is it a variabel?
    variable *A = this->findVariable(valueA);
    if (!A){ error("Undeclared variabel "+valueA); return toReturn; }
    else{ valueA = A->value; }
  }else if (valueA[0] == '['){ valueA.erase(0,1); }

  if (valueB[0] == '$' && valueB[valueB.size()-1] == ']' || valueB[0] == '$'){
    if (valueB[0] == '$' && valueB[valueB.size()-1] != ']'){ valueB.erase(0,1); }
    else{ valueB.erase(0,1); valueB.erase(valueB.size()-1); }

    variable *B = this->findVariable(valueB);
    if (!B){ error("Undeclared variabel "+valueB); return toReturn; }
    else{ valueB = B->value; }
  }else if (valueB[valueB.size()-1] == ']'){ valueB.erase(valueB.size()-1); }

  try{
    if (comparison == "<"){(std::stof(valueA) < std::stof(valueB))? toReturn=true:toReturn=false; }
    else if (comparison == ">"){(std::stof(valueA) > std::stof(valueB))? toReturn=true:toReturn=false; }
    else if (comparison == "<="){(std::stof(valueA) <= std::stof(valueB))? toReturn=true:toReturn=false; }
    else if (comparison == ">="){(std::stof(valueA) >= std::stof(valueB))? toReturn=true:toReturn=false; }
    else if (comparison == "=="){(std::stof(valueA) == std::stof(valueB))? toReturn=true:toReturn=false; }
    else if (comparison == "!="){(std::stof(valueA) != std::stof(valueB))? toReturn=true:toReturn=false; }
    else if (comparison == "in"){ (call_if_IN(valueB, valueA))? toReturn=true:toReturn=false; }
    else{ error("Unknown comparison "+comparison); return toReturn; }

  }catch(std::invalid_argument& e){ error("Strings are not allowed unless your comparison is 'in'"); toReturn = false; }

  valueA = tempA; // Reset
  valueB = tempB; // Reset
  return toReturn;
}

void built::call_system(std::vector<std::string> guts){ for (std::string entry:guts){ system(entry.c_str()); }}

void built::call_leftShift(variable* A, std::string value){
  try{
    A->value = std::to_string(std::stoi(A->value)<<std::stoi(value));
  }catch(std::invalid_argument& e){ error("Both the value of the variable and the value to shift must be ints!"); return; }
}

void built::call_rightShift(variable* A, std::string value){
  try{
    A->value = std::to_string(std::stoi(A->value)>>std::stoi(value));
  }catch(std::invalid_argument& e){ error("Both the value of the variable and the value to shift must be ints!"); return; }
}

// Mathematical operators
std::string built::call_add(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    variable *A = this->findVariable(valueA); // find it
    if (!A){ error("Variable "+valueA+" has not been defined!"); }
    else{ valueA = A->value; }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    variable *B = this->findVariable(valueB); // find it
    if (!B){ error("Variable "+valueB+" has not been defined!"); }
    else{ valueB = B->value; }
  }

  try{
    int iA = std::stoi(valueA);
    int iB = std::stoi(valueB);
    return std::to_string(iA+iB);
  }catch(std::invalid_argument& e){
    return (valueA+valueB);
  }
}

std::string built::call_sub(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    variable *A = this->findVariable(valueA); // find it
    if (!A){ error("Variable "+valueA+" has not been defined!"); }
    else{ valueA = A->value; }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    variable *B = this->findVariable(valueB); // find it
    if (!B){ error("Variable "+valueB+" has not been defined!"); }
    else{ valueB = B->value; }
  }

  int result = 0;
  try{ result = std::stoi(valueA) - std::stoi(valueB); }
  catch(std::invalid_argument& e){ error("Can't subtract a string"); return ""; }
  return std::to_string(result);
}

std::string built::call_div(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    variable *A = this->findVariable(valueA); // find it
    if (!A){ error("Variable "+valueA+" has not been defined!"); }
    else{ valueA = A->value; }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    variable *B = this->findVariable(valueB); // find it
    if (!B){ error("Variable "+valueB+" has not been defined!"); }
    else{ valueB = B->value; }
  }

  float result = 0;
  try{
    if (valueB != "0"){ result = std::stof(valueA) / std::stof(valueB); }
    else{ error ("Division by zero"); return ""; }
  }
  catch(std::invalid_argument& e){ error("Can't divide a string"); return ""; }
  return std::to_string(result);
}

std::string built::call_mod(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    variable *A = this->findVariable(valueA); // find it
    if (!A){ error("Variable "+valueA+" has not been defined!"); }
    else{ valueA = A->value; }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    variable *B = this->findVariable(valueB); // find it
    if (!B){ error("Variable "+valueB+" has not been defined!"); }
    else{ valueB = B->value; }
  }

  int result = 0;
  try{ result = std::stoi(valueA) % std::stoi(valueB); }
  catch(std::invalid_argument& e){ error("Can't modular a string"); return "";}
  return std::to_string(result);
}

std::string built::call_mult(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    variable *A = this->findVariable(valueA); // find it
    if (!A){ error("Variable "+valueA+" has not been defined!"); }
    else{ valueA = A->value; }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    variable *B = this->findVariable(valueB); // find it
    if (!B){ error("Variable "+valueB+" has not been defined!"); }
    else{ valueB = B->value; }
  }

  float result = 0;
  try{ result = std::stof(valueA) * std::stof(valueB); }
  catch(std::invalid_argument& e){ error("Can't multiply a string"); return "";}
  return std::to_string(result);
}

std::string built::call_raise(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    variable *A = this->findVariable(valueA); // find it
    if (!A){ error("Variable "+valueA+" has not been defined!"); }
    else{ valueA = A->value; }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    variable *B = this->findVariable(valueB); // find it
    if (!B){ error("Variable "+valueB+" has not been defined!"); }
    else{ valueB = B->value; }
  }

  int result = 1;
  int floor = 0;
  try{
    floor = std::stoi(valueB);
    while (floor > 0){
      result *= std::stoi(valueA);
      floor--;
    }
  }
  catch(std::invalid_argument& e){ error("Can't raise a string"); return "";}
  return std::to_string(result);
}
