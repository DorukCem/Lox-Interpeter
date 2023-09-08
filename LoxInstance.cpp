#include "headers/LoxInstance.h"
#include "headers/RuntimeError.h"
#include "headers/LoxFunction.h"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> lox_class)
   : lox_class(lox_class)
{}

std::string LoxInstance::to_string() 
{ 
   return lox_class-> name + " instance"; 
}

std::any LoxInstance::get(Token name)
{
   if (fields.find(name.lexeme) != fields.end()) {
      return fields[name.lexeme];
   }

   std::shared_ptr<LoxFunction> method = lox_class->find_method(name.lexeme);
   if (method != nullptr) { 
      return method->bind( shared_from_this() );
   }

   throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}


void LoxInstance::set(Token name, std::any value)
{
   fields[name.lexeme] = value;
}

