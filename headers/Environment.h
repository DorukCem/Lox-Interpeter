#pragma once
#include <unordered_map>
#include <string>
#include <any>
#include <memory>
#include "Token.h"

class Environment: public std::enable_shared_from_this<Environment> {
public:
   std::shared_ptr<Environment> enclosing;
   void define(std::string name, std::any value);
   std::any get(Token name);
   std::any get_at( int distance, std::string name);
   void assign(Token name, std::any value);   
   void assign_at(int distance, Token name, std::any value);   
   std::shared_ptr<Environment> ancestor(int distance);
   Environment();
   Environment(std::shared_ptr<Environment> enclosing);
private:
   std::unordered_map<std::string, std::any> values;
};

