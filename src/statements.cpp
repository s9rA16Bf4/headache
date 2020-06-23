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
        if (splitGuts[i] == "let" && splitGuts[i+2] == "="){ _bt.call_def_var(splitGuts[i+1], splitGuts[i+3], false); break; } // variable dec
        else if (splitGuts[i] == "let" && splitGuts[i+2] == ":="){ _bt.call_def_arr(splitGuts[i+1], splitGuts, i); break; } // array dec
        else if (splitGuts[i] == "const" && splitGuts[i+2] == "="){ _bt.call_def_var(splitGuts[i+1], splitGuts[i+3], true); break; }
        else if (splitGuts[i] == "print"){ _bt.call_print(splitGuts); break;}
        else if (splitGuts[i] == "def" && splitGuts.size() == 3 && splitGuts[i+2] == "{"){ // Defining a function
          _bt.call_def_func(splitGuts[i+1], gatherGuts(p, contents));
          break;
        }
        else if (splitGuts[i] == "inc"){
          bool result = false;
          if (splitGuts.size() == 2){ result = _bt.call_inc(contents, splitGuts[i+1], p, "NULL"); }
          else if (splitGuts.size() == 4){ result = _bt.call_inc(contents, splitGuts[i+1], p, splitGuts[3]); }

          if (result) { p--; }  // sub one to "jump a step up"

          break;
        }
        else if ((splitGuts[i] == "if" || splitGuts[i] == "while") && splitGuts[splitGuts.size()-1] == "{"){
          std::vector<std::string> guts = gatherGuts(p, contents); // This will gather the necessary information
          if (splitGuts[0] == "if"){ _bt.call_if_run(splitGuts, guts); }
          else if (splitGuts[0] == "while"){ _bt.call_while_run(splitGuts, guts); }
          break;
        }
        else if (splitGuts[i] == "system" && splitGuts[i+1] == "{"){ _bt.call_local_system(gatherGuts(p,contents)); break; }
        else if (splitGuts[i] == "exit"){ std::exit(EXIT_FAILURE); } // Exits the program
        else if ((splitGuts[i] == "python") && splitGuts[i+1] == "{"){ _bt.call_local_python("NULL", gatherGuts(p, contents)); break; }
        else if ((splitGuts[i] == "python") && splitGuts[i+2] == "{"){ _bt.call_local_python(splitGuts[i+1], gatherGuts(p, contents)); break; }
        else if ((splitGuts[i] == "asm") && splitGuts[i+1] == "{"){ _bt.call_local_asm("NULL", gatherGuts(p, contents)); break; }
        else if ((splitGuts[i] == "asm") && splitGuts[i+2] == "{"){ _bt.call_local_asm(splitGuts[i+1], gatherGuts(p, contents)); break; }
        else if ((splitGuts[i] == "js") && splitGuts[i+1] == "{"){ _bt.call_local_node("NULL", gatherGuts(p, contents)); break; }
        else if ((splitGuts[i] == "js") && splitGuts[i+2] == "{"){ _bt.call_local_node(splitGuts[i+1], gatherGuts(p, contents)); break; }
        else if (splitGuts[i] == "goto"){ contents[p] = ""; p = _bt.call_goto(p, contents.size(), splitGuts[i+1]) - 1; break;}
        else if (splitGuts[i] == "file" && splitGuts[i+2] == "{"){ _bt.call_local_file(splitGuts[i+1], gatherGuts(p, contents)); break; }
        else if (splitGuts[i] == "read" && _bt.call_local_check_file_operation()){ _bt.call_local_file_read(splitGuts[i+1]); break; }
        else if (splitGuts[i] == "write" && _bt.call_local_check_file_operation()){ _bt.call_local_file_write(splitGuts); break; }
        else if (splitGuts[i] == "delete" && _bt.call_local_check_file_operation()){ _bt.call_local_file_remove(splitGuts[i+1]); break; }


        else{
          variable *A = _bt.findVariable(splitGuts[i]);
          std::string potentialFuncName = splitGuts[i].substr(0, splitGuts[i].size()-2); // This removes the () from the name
          function *B = _bt.findFunction(potentialFuncName);
          array *C = _bt.findArray(_bt.call_get_array_name(splitGuts[i]));

          if ((A || C) && splitGuts[i+1] == "="){
            if (splitGuts.size() == 5){ // Do we have enough for mathematical operations?
              std::string firstVariableOrValue = splitGuts[2], secondVariableOrValue = splitGuts[4];
              if (A && A->constValue){ error("Can't modify a const, you can only read from it!"); break; }
              else{
                if (A){
                  if (splitGuts[3] == "+"){ A->value =_bt.call_add(firstVariableOrValue, secondVariableOrValue); }
                  else if (splitGuts[3] == "-"){ A->value = _bt.call_sub(firstVariableOrValue, secondVariableOrValue); }
                  else if (splitGuts[3] == "/"){ A->value = _bt.call_div(firstVariableOrValue, secondVariableOrValue); }
                  else if (splitGuts[3] == "//"){ A->value = _bt.call_mod(firstVariableOrValue, secondVariableOrValue); }
                  else if (splitGuts[3] == "*"){ A->value = _bt.call_mult(firstVariableOrValue, secondVariableOrValue); }
                  else if (splitGuts[3] == "**"){ A->value = _bt.call_raise(firstVariableOrValue, secondVariableOrValue); }
                  else{ error("Unknown mathematical operator " + splitGuts[3]); }
                }else{
                  std::string index = _bt.call_get_array_index(splitGuts[i]);
                  if (splitGuts[3] == "+"){ _bt.call_array_upd_at_inx(C, index, _bt.call_add(firstVariableOrValue, secondVariableOrValue)); }
                  else if (splitGuts[3] == "-"){ _bt.call_array_upd_at_inx(C, index, _bt.call_sub(firstVariableOrValue, secondVariableOrValue)); }
                  else if (splitGuts[3] == "/"){  _bt.call_array_upd_at_inx(C, index, _bt.call_div(firstVariableOrValue, secondVariableOrValue)); }
                  else if (splitGuts[3] == "//"){  _bt.call_array_upd_at_inx(C, index, _bt.call_mod(firstVariableOrValue, secondVariableOrValue)); }
                  else if (splitGuts[3] == "*"){ _bt.call_array_upd_at_inx(C, index, _bt.call_mult(firstVariableOrValue, secondVariableOrValue)); }
                  else if (splitGuts[3] == "**"){  _bt.call_array_upd_at_inx(C, index, _bt.call_raise(firstVariableOrValue, secondVariableOrValue)); }
                  else{ error("Unknown mathematical operator " + splitGuts[3]); }
                }
              }
              break;
            }
            else if (A){ _bt.call_upd_var(A, splitGuts[i+2]); break; } // Update the variable
            else if (C){ _bt.call_array_upd_at_inx(C, _bt.call_get_array_index(splitGuts[i]) ,splitGuts[i+2]); break; }
          }
          else if (A && splitGuts[i+1] == "<<"){ _bt.call_leftShift(A, splitGuts[i+2]); break; }
          else if (A && splitGuts[i+1] == ">>"){ _bt.call_rightShift(A, splitGuts[i+2]); break;}
          else if (B){ _bt.call_run_func(B); break; } // Call the function
          else{ error("Unknown syntax "+splitGuts[i]+""); }
        }
      }
    }
  }
 }
