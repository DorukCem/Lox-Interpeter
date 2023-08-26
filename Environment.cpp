#include "headers/Environment.h"
#include "headers/RuntimeError.h"

void Environment::define(std::string name, std::any value)
{
   values[name] = value;
}

std::any Environment::get(Token name)
{
   if (values.find(name.lexeme) != values.end())
   {
      return values[name.lexeme];
   }
      
   throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
