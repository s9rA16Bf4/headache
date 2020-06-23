#ifndef BUILTINFUNCTIONS_HPP
#define BUILTINFUNCTIONS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "globalValues.hpp"
#include "errorHandling.hpp"
#include "process.hpp"
#include "statements.hpp"

// This is where all the corresponding functions for our syntaxes are being held

class built{
private:
  static bool fileOperation;
  static bool pythonOperation;
  static bool asmOperation;
  static bool nodeOperation;
  static bool sqlOperation;

public:
  void call_print(std::vector<std::string>); // Prints a message to the screen and/or the contents of a variable
  void call_def_var(std::string, std::string, bool); // Defines a classless variable
  void call_def_arr(std::string, std::vector<std::string>, const int); // Defines an array
  void call_def_func(std::string, std::vector<std::string>);

  std::vector<std::string> call_include(std::string); // Includes the contents of the file specificed, else raises an error
  void call_upd_var(variable*, std::string); // Updates the value of a variable, either with another value or the guts of another variable
  void call_run_func(function*); // Runs a function
  bool call_inc(std::vector<std::string>&, std::string, const int, std::string); // includes the all the contents of a file

  bool call_statement_check(std::string&, std::string&, std::string);
  bool call_if_IN(std::string, std::string); // Investigates if 'word' exists in 'message'
  void call_if_run(std::vector<std::string>, std::vector<std::string>); // Checks if the if is valid and runs what's supposed to run
  void call_while_run(std::vector<std::string>, std::vector<std::string>); // Checks if the while loop is valid and runs what's supposed to run

  void call_leftShift(variable*, std::string);
  void call_rightShift(variable*, std::string);

  std::string call_array_value_at_inx(std::string); // Returns the value at a certain index
  void call_array_upd_at_inx(array*, std::string, std::string); // Updates the value at a certain index
  std::string call_get_array_name(std::string); // Gets the name of the array, $<index>$<arrayName>
  std::string call_get_array_index(std::string); // Gets the index, $<index>$<arrayName>
  int call_array_size(array*); // Returns the arrays size

  void call_local_system(std::vector<std::string>); // Executes a command on the computer
  void call_local_python(std::string, std::vector<std::string>); // Runs python code
  void call_local_asm(std::string, std::vector<std::string>); // Runs asm code, needs nasm to work
  void call_local_node(std::string, std::vector<std::string>); // Runs node code
  std::string call_local_find_var(std::string, const char); // Takes a string and finds out if it contains a variable

  unsigned int call_goto(const unsigned int, const unsigned int,std::string); // Changes the flow and jumps to a specific line

  void call_local_file(std::string, std::vector<std::string>); // Manipulates a local file which is provided by the user
  void call_local_file_write(std::vector<std::string>); // Has control of which values will later be saved to the harddrive
  void call_local_file_read(std::string); // Reads all the lines of a file
  void call_local_file_remove(std::string); // Removes a line from a file
  void call_local_file_open(std::string); // Opens a file and reads all it's contents which is then returned to the user
  void call_local_file_save(std::string); // Saves all the values to the locally provided file

  // Checks if different operations has been active
  bool call_local_check_file_operation();
  bool call_local_check_sql_operation();
  bool call_local_check_asm_operation();
  bool call_local_check_js_operation();
  bool call_local_check_python_operation();

  std::string call_add(std::string, std::string); // Adds two values
  std::string call_sub(std::string, std::string); // Subtracts the value of one with a another
  std::string call_div(std::string, std::string); // Divides two values
  std::string call_mod(std::string, std::string); // Modular two values
  std::string call_mult(std::string, std::string); // Multiplies two values
  std::string call_raise(std::string, std::string); // Raises two values together

  variable* findVariable(std::string); // Finds the variable assocciated with the name, else nullptr
  function* findFunction(std::string); // Finds the function assocciated with the name, else nullptr
  array* findArray(std::string); // Finds the array assocciated with the name, else nullptr
};

#endif
