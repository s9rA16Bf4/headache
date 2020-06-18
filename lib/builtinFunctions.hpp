#ifndef BUILTINFUNCTIONS_HPP
#define BUILTINFUNCTIONS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "globalValues.hpp"
#include "errorHandling.hpp"
#include "process.hpp"
#include "statements.hpp"

// This is where all the corresponding functions for our syntaxes are being held

class built{
public:
  bool call_if_IN(std::string, std::string); // Investigates if 'word' exists in 'message'
  std::vector<std::string> call_for_IN(std::string, std::string); // Returns a vector consisting of word size strings
  void call_print(std::vector<std::string>); // Prints a message to the screen and/or the contents of a variable
  void call_def_var(std::string, std::string, bool); // Defines a classless variable
  void call_def_func(std::string, std::string, std::vector<std::string>);
  std::vector<std::string> call_include(std::string); // Includes the contents of the file specificed, else raises an error
  void call_upd_var(variable*, std::string); // Updates the value of a variable, either with another value or the guts of another variable
  void call_run_func(function*);
  bool call_inc(std::vector<std::string>&, std::string, const int, std::string); // includes the all the contents of a file
  bool call_statement_check(std::string&, std::string&, std::string);
  void call_if_run(std::vector<std::string>, std::vector<std::string>); // Checks if the if is valid and runs what's supposed to run
  void call_while_run(std::vector<std::string>, std::vector<std::string>); // Checks if the while loop is valid and runs what's supposed to run
  void call_system(std::vector<std::string>); // Executes a command on the computer

  std::string call_add(std::string, std::string); // Adds two values together were the none of them are variables
  std::string call_sub(std::string, std::string); // Subtracts two values together were the none of them are variables
  std::string call_div(std::string, std::string); // Divides two values together were the second one is a variable
  std::string call_mod(std::string, std::string); // Modular two values together were the none of them are variables
  std::string call_mult(std::string, std::string); // Multiplies two values together were neither are variables
  std::string call_raise(std::string, std::string); // Raises two values together were neither are variables

  variable* findVariable(std::string); // Finds the variable assocciated with the name, else nullptr
  function* findFunction(std::string); // Finds the function assocciated with the name, else nullptr
};

#endif
