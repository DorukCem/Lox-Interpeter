#pragma once
#include <string>
#include "LoxCallable.h"
#include <memory>

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass>{
public:
   LoxClass(std::string name) : name(name) {}
   const std::string name;
public:
   std::string to_string() override { return name; }
   std::any call(Interpreter& interpeter, std::vector<std::any> arguments) override;
   int arity();
};
