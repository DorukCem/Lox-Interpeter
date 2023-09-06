#include "headers/LoxClass.h"
#include "headers/LoxInstance.h"

std::any LoxClass::call(Interpreter& interpeter, std::vector<std::any> arguments)
{
   auto instance = std::make_shared<LoxInstance>(shared_from_this());
   return instance;
}

int LoxClass::arity()
{
   return 0;
}