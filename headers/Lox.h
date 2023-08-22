#pragma once
#include <string>
#include "Token.h"
#include "RuntimeError.h"
#include "Interpreter.h"

class Lox
{
public:
  static void run_script(int argc, char const *argv[]);
  static void error(int line, std::string message);
  static void error(Token token, std::string message);
  static void runtime_error(RuntimeError error);
private:
  static bool had_error;
  static bool had_runtime_error;
  static Interpreter interpreter;
private:
  static void run_file(std::string path); 
  static void run_prompt();
  static void run(std::string source);
  static void report(int line, std::string where,  std::string message);
};

