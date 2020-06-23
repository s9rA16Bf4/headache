#include "builtinFunctions.hpp"

#ifdef __unix__
  #define OS_WINDOWS 0
#elif defined(_WIN32) || defined(WIN32)
  #define OS_WINDOWS 1
#endif

// Private
bool built::fileOperation = false;
bool built::pythonOperation = false;
bool built::asmOperation = false;
bool built::nodeOperation = false;
bool built::sqlOperation = false;

// Public
void built::call_print(std::vector<std::string> listToPrint){
  for (unsigned int i = 1; i < listToPrint.size(); i++){
    std::string message = listToPrint[i];

    if (message == "%" || message[0] == '%'){ break; }
    if (message[0] == '$'){ // Varible found!
      message = message.substr(1); // Remove both the wings from 'message'

      variable *A = this->findVariable(message);
      if (!A){
        if (message.find("$") != std::string::npos){ std::cout << this->call_array_value_at_inx(message); } // is it perhaps an array?
        else{ error("Variable "+message+" has not been declared!"); }
      }
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
      if (value.find("$") != std::string::npos){ value = call_array_value_at_inx(value); }
      else{
        variable *B = this->findVariable(value); // find the value
        if (B){
          if (B->value == "rN"){ value = std::to_string(std::rand() % 10 + 1); }
          else{ value = B->value; }
        }
        else{ error("The variable "+value+" has not been declared!"); return; }
      }
    }

    A = new variable();
    A->name = name;
    A->value = value;
    A->constValue = constValue;
    variables.push_back(A); // Save the variable

  }else{ error("Variable "+name+" has already been declared!"); return; }
}
void built::call_def_arr(std::string name, std::vector<std::string> values, const int index){
  array *A = this->findArray(name);

  if (!A){
    A = new array();
    A->name = name;
    for (int i = index+3; i < values.size(); i++){
      if (values[i] == "%"){ break; }
      else{ A->gut.push_back(values[i]); } }
    arrays.push_back(A);
  }else{ error("An array with the name, "+name+", already exists!"); }
}
void built::call_def_func(std::string name, std::vector<std::string> guts){
  function *A = this->findFunction(name);

  if (!A){
    A = new function();
    A->name = name;
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

  if (!B){
    if (varOrValue.find("$") != std::string::npos){
      std::string result = this->call_array_value_at_inx(varOrValue); //!!!!
      if (result != ""){ A->value = result; }
      else{ A->value = varOrValue; }
    }
    else{ A->value = varOrValue; }  // its a value
  }
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

  if (name[0] == '$'){ name.erase(0,1); }

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
array* built::findArray(std::string name){
  array* A = nullptr;

  for (array* arr:arrays){
    if (arr->name == name){
      A = arr;
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

    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{ // its a variable
      variable *A = this->findVariable(valueA);
      if (!A){ error("Undeclared variabel "+valueA); return toReturn; }
      else{ valueA = A->value; }
    }
  }else if (valueA[0] == '['){ valueA.erase(0,1); }

  if (valueB[0] == '$' && valueB[valueB.size()-1] == ']' || valueB[0] == '$'){
    if (valueB[0] == '$' && valueB[valueB.size()-1] != ']'){ valueB.erase(0,1); }
    else{ valueB.erase(0,1); valueB.erase(valueB.size()-1); }

    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{ // no its a variable
      variable *B = this->findVariable(valueB);
      if (!B){ error("Undeclared variabel "+valueB); return toReturn; }
      else{ valueB = B->value; }
    }
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

std::string built::call_array_value_at_inx(std::string arrayName){
  std::string index = this->call_get_array_index(arrayName), arrName = this->call_get_array_name(arrayName), toReturn = "";

  array *A = this->findArray(arrName);
  if (!A){ error("Undeclared array called "+arrName); }
  else{
    int indexInt = 0;
    variable *B = this->findVariable(index);
    if (B){ index = B->value; }

    try{ indexInt = std::stoi(index); }
    catch(std::invalid_argument& e){ error("The index you're trying to reach must be an int!"); return ""; }
    if (indexInt > A->gut.size()-1 || indexInt < 0){ error("The index is out of bounds!"); }
    else{ toReturn = A->gut[indexInt]; }
  }

  return toReturn;
}
void built::call_array_upd_at_inx(array *A, std::string index, std::string value){
  int indexInt = 0;

  variable *B = this->findVariable(index); // Are these variables?
  variable *C = this->findVariable(value);
  if (B){ index = B->value; }
  if (C){ value = C->value; }
  else{ // Might be an array
    std::string getIndex = this->call_get_array_index(value);
    array* D = this->findArray(this->call_get_array_name(value));
    if (D){
      variable *E = this->findVariable(getIndex);
      if (E){ getIndex = E->value; }

      try{ indexInt = std::stoi(getIndex); } // Tries to convert it to an int
      catch(std::invalid_argument& e){ error("Index can't be a string and must be an int"); return; }
      if (indexInt > A->gut.size()-1 || indexInt < 0){  error("The index is out of bounds"); return;}
      value = D->gut[indexInt];
    }
  }

  try{ indexInt = std::stoi(index); } // Tries to convert it to an int
  catch(std::invalid_argument& e){ error("Index can't be a string and must be an int"); return; }
  if (indexInt > A->gut.size()-1 || indexInt < 0){ error("The index is out of bounds"); return;}

  A->gut[indexInt] = value; // And update
}

std::string built::call_get_array_name(std::string name){
  std::size_t prevPos = 0, pos = 0;
  bool run = true;
  while(run){
    pos = name.find("$");
    if (pos != std::string::npos){
      name.erase(prevPos, pos+1);
      prevPos = pos;
    }else{ run = false; }
  }
  return name;
}
std::string built::call_get_array_index(std::string name){
  std::string index = "";
  unsigned int i = 0;
  if (name[0] == '$'){ i++; } // jump a step
  while(name[i] != '$'){ index += name[i]; i++; }
  return index;
}

std::string built::call_local_find_var(std::string entry, const char endDelimiter){
  std::string potVariable = "", value = "", originalEntry = entry;
  unsigned int potVarStart = 0;
  bool varArray = false, modified = false;

  for (unsigned int i = 0; i < entry.size(); i++){
      if (entry[i] == '$'){ // Pot a variable
        potVarStart = i; // Remember where we started
        while(entry[++i] != endDelimiter){ potVariable += entry[i]; } // Get the variable name

        if (potVariable.find("$") != std::string::npos){ // its an array
          array *A = this->findArray(this->call_get_array_name(potVariable));
          if (A){ value = this->call_array_value_at_inx(potVariable); varArray = true; }

        }
        else{ // its a variable
          variable *A = this->findVariable(potVariable);
          if (A){
            value = A->value;
            if (A->value == "nL") { value = "\n"; }
            else if (A->value == "tL") { value = "\t"; }
            varArray = true;
          }
        }
        if (varArray){ entry.replace(potVarStart, potVariable.size()+2, value); modified = true; } // Insert the value of our variable/array
        potVariable = ""; // Reset
        varArray = false; // Reset
    }
  }

  if (!modified){ entry = originalEntry; }

  return entry;
}

void built::call_local_python(std::string fileName, std::vector<std::string> guts){
  this->pythonOperation = true;
  process _p;

  if (fileName == "NULL"){ fileName = "tmpFile"; }

  std::ofstream openFile(fileName, std::ios_base::app);
  std::vector<std::string> localGuts;
  std::string systemCommand = "python " + fileName, systemCommandRM = "", line = "";


  if (OS_WINDOWS) { systemCommandRM = "del " + fileName; } //Windows based systems
  else{ systemCommandRM =  "rm " + fileName; } // Nix based

  if (openFile.is_open()){
      for (std::string entry:guts){
        entry = this->call_local_find_var(entry, '/');
        openFile << entry << std::endl; // Write the line
      }

      openFile.close(); // Close the file
      std::system(systemCommand.c_str()); // Run the file
      if (fileName == "tmpFile"){ std::system(systemCommandRM.c_str()); } // Remove the file as its only a temp file

    }else{ error("Failed to open file"); }
    this->pythonOperation = false;
}
void built::call_local_asm(std::string fileName, std::vector<std::string> guts){
  this->asmOperation = true;
  process _p;

  if (fileName == "NULL"){ fileName = "tmpFile"; }

  std::ofstream openFile(fileName, std::ios_base::app);
  std::vector<std::string> localGuts;
  std::string systemCommand = "nasm -f elf64 " + fileName + " -o tmp.o && ld tmp.o -o tmp && ./tmp", systemCommandRM = "", line = "";


  if (OS_WINDOWS) { systemCommandRM = "del tmpFile"; } //Windows based systems
  else{ systemCommandRM =  "rm tmpFile"; } // Nix based

  if (openFile.is_open()){
      for (std::string entry:guts){
        entry = this->call_local_find_var(entry, '/');
        openFile << entry << std::endl; // Write the line
      }

      openFile.close(); // Close the file
      std::system(systemCommand.c_str()); // Run the file
      if (fileName == "tmpFile"){ std::system(systemCommandRM.c_str()); } // Remove the file as its only a temp file

      // Extra to remove
      if (OS_WINDOWS){ std::system("del tmp tmp.o");}
      else{ std::system("rm tmp tmp.o"); }

    }else{ error("Failed to open file"); }
    this->asmOperation = false;
}
void built::call_local_node(std::string fileName, std::vector<std::string> guts){
  this->nodeOperation = true;
  process _p;

  if (fileName == "NULL"){ fileName = "tmpFile"; }

  std::ofstream openFile(fileName, std::ios_base::app);
  std::vector<std::string> localGuts;
  std::string systemCommand = "node " + fileName, systemCommandRM = "", line = "";


  if (OS_WINDOWS) { systemCommandRM = "del " + fileName; } //Windows based systems
  else{ systemCommandRM =  "rm " + fileName; } // Nix based

  if (openFile.is_open()){
      for (std::string entry:guts){
        entry = this->call_local_find_var(entry, '/');
        openFile << entry << std::endl; // Write the line
      }

      openFile.close(); // Close the file
      std::system(systemCommand.c_str()); // Run the file
      if (fileName == "tmpFile"){ std::system(systemCommandRM.c_str()); } // Remove the file as its only a temp file

    }else{ error("Failed to open file"); }
    this->nodeOperation = false;
}
void built::call_local_system(std::vector<std::string> guts){ for (std::string entry:guts){ system(entry.c_str()); }}

unsigned int built::call_goto(const unsigned int oldValue, const unsigned int maxValue ,std::string lineToGo){
  int toReturn = oldValue, lineToGoInt;

  try{ lineToGoInt = std::stoi(lineToGo); }
  catch(std::invalid_argument& e){ error("Line must be a int"); return toReturn; }
  if ((lineToGoInt-1) >= 0 && lineToGoInt-1 < maxValue){ toReturn = lineToGoInt - 1; }
  else{ error("Line must be positive and can't be larger than "+std::to_string(maxValue)); return toReturn; }

  return toReturn;
}

void built::call_local_file(std::string fileName, std::vector<std::string> gut){
  this->fileOperation = true;
  this->call_local_file_open(fileName);
  statements(gut); // This will manipulate the file
  this->fileOperation = false;
  this->call_local_file_save(fileName); // Save the file
}
void built::call_local_file_write(std::vector<std::string> values){
  std::string line = "";
  for (unsigned int i = 1; i < values.size(); i++){
    if (values[i] == "%"){ break; }
    if (values[i][0] == '$'){ values[i] = this->call_local_find_var(values[i], '/'); } // Checking if its a variable or an array
    line += values[i] + " ";
  }
  fileGut.push_back(line);
}
void built::call_local_file_read(std::string value){
  if (value != "all"){
    int index = 0;
    try{ index = std::stoi(value) - 1; }
    catch(std::invalid_argument& e){ error("Index must be a int"); return; }
    if (index > (fileGut.size()-1) || index < 0 || fileGut.size() == 0) { error("Index is out of bounds"); return; }
    else { std::cout << fileGut[index]; }

  }else{ for(std::string entry:fileGut){ std::cout << entry << std::endl; } }
}
void built::call_local_file_remove(std::string value){
  try{
    int index = std::stoi(value) - 1;
    fileGut.erase(fileGut.begin()+index);
  }catch(std::invalid_argument& e){ // might be a value
    auto index = std::find(fileGut.begin(), fileGut.end(), value);
    if (index == fileGut.end()){ error("Value "+value+ " could not be found"); return; }
    else{ fileGut.erase(index); }
  }
}
void built::call_local_file_open(std::string value){
  std::ifstream openFile(value);
  std::string line = "";

  if (openFile.is_open()){ while(std::getline(openFile, line)){ fileGut.push_back(line); } openFile.close(); }
  else{ error("Failed to open file"); }
}
void built::call_local_file_save(std::string fileName){
  std::ofstream openFile(fileName);

  if (openFile.is_open()){ for(std::string line:fileGut){ openFile << line << std::endl; } openFile.close(); }
  else{ error("Failed to open file"); }
}

bool built::call_local_check_file_operation(){ return this->fileOperation; }
bool built::call_local_check_sql_operation(){ return this->sqlOperation; }
bool built::call_local_check_asm_operation(){ return this->asmOperation; }
bool built::call_local_check_js_operation(){ return this->nodeOperation; }
bool built::call_local_check_python_operation(){ return this->pythonOperation; }

// Mathematical operators
std::string built::call_add(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{
      variable *A = this->findVariable(valueA); // find it
      if (!A){ error("Variable "+valueA+" has not been defined!"); }
      else{ valueA = A->value; }
    }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{
      variable *B = this->findVariable(valueB); // find it
      if (!B){ error("Variable "+valueB+" has not been defined!"); }
      else{ valueB = B->value; }
    }
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
    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{
      variable *A = this->findVariable(valueA); // find it
      if (!A){ error("Variable "+valueA+" has not been defined!"); }
      else{ valueA = A->value; }
    }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{
      variable *B = this->findVariable(valueB); // find it
      if (!B){ error("Variable "+valueB+" has not been defined!"); }
      else{ valueB = B->value; }
    }
  }

  int result = 0;
  try{ result = std::stoi(valueA) - std::stoi(valueB); }
  catch(std::invalid_argument& e){ error("Can't subtract a string"); return ""; }
  return std::to_string(result);
}
std::string built::call_div(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{
      variable *A = this->findVariable(valueA); // find it
      if (!A){ error("Variable "+valueA+" has not been defined!"); }
      else{ valueA = A->value; }
    }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{
      variable *B = this->findVariable(valueB); // find it
      if (!B){ error("Variable "+valueB+" has not been defined!"); }
      else{ valueB = B->value; }
    }
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
    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{
      variable *A = this->findVariable(valueA); // find it
      if (!A){ error("Variable "+valueA+" has not been defined!"); }
      else{ valueA = A->value; }
    }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{
      variable *B = this->findVariable(valueB); // find it
      if (!B){ error("Variable "+valueB+" has not been defined!"); }
      else{ valueB = B->value; }
    }
  }

  int result = 0;
  try{ result = std::stoi(valueA) % std::stoi(valueB); }
  catch(std::invalid_argument& e){ error("Can't modular a string"); return "";}
  return std::to_string(result);
}
std::string built::call_mult(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{
      variable *A = this->findVariable(valueA); // find it
      if (!A){ error("Variable "+valueA+" has not been defined!"); }
      else{ valueA = A->value; }
    }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{
      variable *B = this->findVariable(valueB); // find it
      if (!B){ error("Variable "+valueB+" has not been defined!"); }
      else{ valueB = B->value; }
    }
  }

  float result = 0;
  try{ result = std::stof(valueA) * std::stof(valueB); }
  catch(std::invalid_argument& e){ error("Can't multiply a string"); return "";}
  return std::to_string(result);
}
std::string built::call_raise(std::string valueA, std::string valueB){
  if (valueA[0] == '$'){ // is valueA a variable?
    valueA.erase(0,1); // Remove the dollar sign
    if (valueA.find("$") != std::string::npos){ valueA = this->call_array_value_at_inx(valueA); } // its an array
    else{
      variable *A = this->findVariable(valueA); // find it
      if (!A){ error("Variable "+valueA+" has not been defined!"); }
      else{ valueA = A->value; }
    }
  }

  if (valueB[0] == '$'){ // is valueB a variable?
    valueB.erase(0,1); // Remove the dollar sign
    if (valueB.find("$") != std::string::npos){ valueB = this->call_array_value_at_inx(valueB); } // its an array
    else{
      variable *B = this->findVariable(valueB); // find it
      if (!B){ error("Variable "+valueB+" has not been defined!"); }
      else{ valueB = B->value; }
    }
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
