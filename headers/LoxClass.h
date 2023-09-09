#pragma once
#include <string>
#include "LoxCallable.h"
#include <memory>
#include "LoxFunction.h"
#include <map>

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass>{
public:
   LoxClass(std::string name, std::shared_ptr<LoxClass> superclass,std::map<std::string, std::shared_ptr<LoxFunction>> methods) 
      : name(name), superclass(superclass), methods(methods) {}
   const std::string name;
   const std::shared_ptr<LoxClass> superclass;
   std::map<std::string, std::shared_ptr<LoxFunction>> methods; //! This is potentially a huge copy operation
public:
   std::string to_string() override { return name; }
   std::any call(Interpreter& interpeter, std::vector<std::any> arguments) override;
   int arity();
   std::shared_ptr<LoxFunction> find_method(std::string name);
};
