#pragma once
#include <unordered_map>
#include <string>
#include <any>
#include "Token.h"

class Environment {
public:
   void define(std::string name, std::any value);
   std::any get(Token name);   
private:
   std::unordered_map<std::string, std::any> values;
};