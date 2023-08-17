#include "headers/Lox.h"
#include "headers/Scanner.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream> 
#include <vector>


bool Lox::had_error = false;

void Lox::run_script(int argc, char const *argv[])
{
   if (argc > 2) {
      std::cout << "Usage: jlox [script]" << std::endl;
      std::exit(64);
   } 
   else if (argc == 2) {
      std::cout << "Running from file at: " << argv[1] << std::endl;
      run_file(argv[1]);
   }
   else {
      run_prompt();
   }
}

// ? If this method of reading strings is too slow we may need to update the method
void Lox::run_file(std::string path)
{
   std::ifstream file(path);
   std::stringstream string_buffer;
   string_buffer << file.rdbuf();
   run(string_buffer.str());

   if (had_error){
      exit(65);
   }
}

void Lox::run_prompt()
{
   std::string input;
   while(1)
   {
      std::cout << ">";
      std::getline(std::cin, input);
      if (input == "exit") { 
         std::cout << "terminated";
         break;
      }
      run(input);
   }
}

void Lox::run(std::string source)
{
   Scanner scanner(source);
   std::vector<Token> tokens = scanner.scan_tokens();

    // For now, just print the tokens.
   for (Token token : tokens) {
      std::cout << token.to_string() << " ";
   }
   std::cout << tokens.size() << std::endl;
   std::cout << std::endl;
   had_error = false;
}

void Lox::error(int line, std::string message)
{
   report(line, "", message);
}

void Lox::report(int line, std::string where,  std::string message)
{
   std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
   had_error = true;
}