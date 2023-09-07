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

std::shared_ptr<LoxFunction> LoxClass::find_method(std::string name)
{
   if (methods.find(name) != methods.end())
   {
      return methods[name];
   }
   return nullptr;
}