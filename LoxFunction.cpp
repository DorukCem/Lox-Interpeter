#include "headers/LoxFunction.h"
#include "headers/Environment.h"
#include "headers/Interpreter.h"
#include "headers/RuntimeError.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration,  std::shared_ptr<Environment> closure)
   :declaration(declaration), closure(closure)
{ }

std::any LoxFunction::call(Interpreter& interpeter, std::vector<std::any> arguments)
{
   std::shared_ptr<Environment> environment = closure; // ! Might have a problem here in the future
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