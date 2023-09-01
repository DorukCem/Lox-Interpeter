#pragma once
#include "LoxCallable.h"
#include "Statement.h"

class LoxFunction : public LoxCallable 
{
public:
   int arity() override;
   std::string to_string() override;
   std::any call(Interpreter& interpeter, std::vector<std::any> arguments) override;
   LoxFunction(std::shared_ptr<Function> declaration);
private:
   std::shared_ptr<Function> declaration;
};