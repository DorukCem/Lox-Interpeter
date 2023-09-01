#pragma once

#include <any>
#include <string>
#include <vector>

class Interpreter;

class LoxCallable
{
public:
   virtual int arity() = 0;
   virtual std::string to_string() = 0;
   virtual std::any call(Interpreter& interpeter, std::vector<std::any> arguments) = 0;
   virtual ~LoxCallable() = default;
};