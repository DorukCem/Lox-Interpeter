#include "headers/LoxFunction.h"
#include "headers/Environment.h"
#include "headers/Interpreter.h"
#include "headers/RuntimeError.h"
#include "headers/LoxInstance.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration,  std::shared_ptr<Environment> closure)
   :declaration(declaration), closure(closure)
{ }

std::any LoxFunction::call(Interpreter& interpeter, std::vector<std::any> arguments) 
{
   auto environment = std::make_shared<Environment>(closure); 
   for (int i = 0; i < static_cast<int>(declaration->params.size()); i++)
   {
      environment->define(declaration->params[i].lexeme, arguments[i]);
   }
   try {
      interpeter.execute_block(std::vector<std::shared_ptr<Stmt>>{declaration->body}, environment);
   } catch (LoxReturn return_value) {
      return return_value.value;
   }
   return nullptr;
}

int LoxFunction::arity()
{
   return declaration->params.size();
}

std::string LoxFunction::to_string()
{
   return "<fn " + declaration->name.lexeme + ">";
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance)
{
   auto environment = std::make_shared<Environment>(closure);
   environment->define("this", instance);
   return std::make_shared<LoxFunction>(declaration, environment);
}