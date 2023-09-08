#include "headers/LoxClass.h"
#include "headers/LoxInstance.h"

std::any LoxClass::call(Interpreter& interpeter, std::vector<std::any> arguments)
{
   auto instance = std::make_shared<LoxInstance>(shared_from_this());
   std::shared_ptr<LoxFunction> initializer = find_method("init");
   if (initializer != nullptr) {
      initializer->bind(instance)->call(interpeter, arguments);
   }
   return instance;
}

int LoxClass::arity()
{
   std::shared_ptr<LoxFunction> initializer = find_method("init");
   if (initializer == nullptr) {
      return 0;
   }
   return initializer->arity();
}

std::shared_ptr<LoxFunction> LoxClass::find_method(std::string name)
{
   if (methods.find(name) != methods.end())
   {
      return methods[name];
   }
   return nullptr;
}