#pragma once
#include "LoxClass.h"
#include "memory"
#include "Token.h"
#include <map>

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
public:
   LoxInstance(std::shared_ptr<LoxClass> lox_class);
   std::string to_string(); 
   std::any get(Token name); 
   void set(Token name, std::any value);

private:
   std::shared_ptr<LoxClass> lox_class;
   std::map<std::string, std::any> fields;
};