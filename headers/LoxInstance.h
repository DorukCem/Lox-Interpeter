#pragma once
#include "LoxClass.h"
#include "memory"

class LoxInstance {
public:
   LoxInstance(std::shared_ptr<LoxClass> lox_class) 
      : lox_class(lox_class) {}

   std::string to_string() { return lox_class-> name + " instance"; }
private:
   std::shared_ptr<LoxClass> lox_class;

};