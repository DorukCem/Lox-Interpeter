#pragma once
#include <string>

class Lox
{
public:
  static void run_script(int argc, char const *argv[]);
  static void error(int line, std::string message);
private:
  static bool had_error;
private:
  static void run_file(std::string path); 
  static void run_prompt();
  static void run(std::string source);
  static void report(int line, std::string where,  std::string message);
};