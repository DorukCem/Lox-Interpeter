#include "headers/LoxFunction.h"
#include "headers/Environment.h"
#include "headers/Interpreter.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration)
   :declaration(declaration)
{ }


std::any LoxFunction::call(Interpreter& interpeter, std::vector<std::any> arguments)
{
   auto environment = std::make_shared<Environment>(interpeter.global_environment);
   for (int i = 0; i < static_cast<int>(declaration->params.size()); i++)
   {
      environment->define(declaration->params[i].lexeme, arguments[i]);
   }

   interpeter.execute_block(std::vector<std::shared_ptr<Stmt>>{declaration->body}, environment);
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