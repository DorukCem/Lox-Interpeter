#pragma once
#include "LoxCallable.h"
#include "Statement.h"
#include "Environment.h"


class LoxInstance;

class LoxFunction : public LoxCallable 
{
public:
   int arity() override;
   std::string to_string() override;
   std::any call(Interpreter& interpeter, std::vector<std::any> arguments) override;
   std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);
   LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure);
private:
   std::shared_ptr<Function> declaration;
   std::shared_ptr<Environment> closure;
   
};