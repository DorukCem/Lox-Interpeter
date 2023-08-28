#include "headers/Environment.h"
#include "headers/RuntimeError.h"

Environment::Environment()
   :enclosing(nullptr)
{}

Environment::Environment(std::shared_ptr<Environment> enclosing)
   :enclosing(enclosing)
{ }

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

   //* If the variable isn’t found in this environment, we recursively try the enclosing one.
   if (enclosing != nullptr) { return enclosing->get(name); }
      
   throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, std::any value)
{
   if (values.find(name.lexeme) != values.end())
   {
      values[name.lexeme] = value;
      return;
   }

   if (enclosing != nullptr) 
   {
      //* if the variable isn’t in this environment, it checks the outer one, recursively.
      enclosing->assign(name, value);
      return;
   }
   
   throw RuntimeError(name, "Undefined variable '"+ name.lexeme + "'."); 
}