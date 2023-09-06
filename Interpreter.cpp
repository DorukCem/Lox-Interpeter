#include "headers/LoxFunction.h"
#include "headers/Lox.h"
#include "headers/LoxClass.h"
#include "headers/LoxInstance.h"
#include <iostream>

Interpreter::Interpreter()
{
   global_environment->define("clock", std::shared_ptr<NativeClock>{});

}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements)
{
   try 
   {
      for (std::shared_ptr<Stmt> statement : statements) {
         execute(statement);
      }

   } catch (RuntimeError const& error) {
      Lox::runtime_error(error);
   }
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt)
{
   stmt->accept(*this);
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
   return expr->accept(*this);
}

void Interpreter::execute_block(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> a_environment)
{
   std::shared_ptr<Environment> previous = this->environment;
   try {
      this->environment = a_environment;

      for (std::shared_ptr<Stmt> stmt : statements){
         execute(stmt);
      }
   } catch(...) // *-> Catch anything
   {
      this->environment = previous; //*-> this block always executes and then throw the error again
      throw;
   }

   this->environment = previous;
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth)
{
   locals[expr] = depth;
}

std::any Interpreter::visit_BinaryExpr(std::shared_ptr<Binary> expr)
{
   std::any right = evaluate(expr->right);
   std::any left  = evaluate(expr->left);

   switch (expr->op.type)
   {
      case GREATER:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) >  std::any_cast<double>(right);
      case GREATER_EQUAL:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) >= std::any_cast<double>(right);
      case LESS:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) <  std::any_cast<double>(right);
      case LESS_EQUAL:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) <= std::any_cast<double>(right);
      case BANG_EQUAL: 
         return !is_equal(left, right);
      case EQUAL_EQUAL: 
         return !is_equal(left, right);
      case MINUS:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) -  std::any_cast<double>(right);
      case SLASH:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) /  std::any_cast<double>(right);
      case STAR:
         assert_number_operands(expr->op, left, right);
         return std::any_cast<double>(left) *  std::any_cast<double>(right);
      
      case PLUS:
         if (left.type() == typeid(double) && right.type() == typeid(double)) 
         {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
         } 

         if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) 
         {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
         }
         throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
      
      default:  
         return nullptr;
   }
}

std::any Interpreter::visit_GroupExpr(std::shared_ptr<Group> expr)
{
   return evaluate(expr->expr_in);
}

std::any Interpreter::visit_LiteralExpr(std::shared_ptr<Literal> expr)
{
   return expr->value;
}

std::any Interpreter::visit_LogicalExpr(std::shared_ptr<Logical> expr)
{
   std::any left = evaluate(expr->left);

   if (expr->op.type == TokenType::OR)
   {
      if (is_truthy(left)) { return left; }
   }
   else
   {
      if (!is_truthy(left)) { return left; }
   }

   return evaluate(expr->right);
}

std::any Interpreter::visit_UnaryExpr(std::shared_ptr<Unary> expr)
{
   std::any right = evaluate(expr->right);

   switch (expr->op.type)
   {
   case MINUS:
      assert_number_operand(expr->op, right);
      return std::any_cast<double>(right) * -1;
   case BANG:
      return !is_truthy(right); 
   default:
      return nullptr;
   }
}

std::any Interpreter::visit_VariableExpr( std::shared_ptr<Variable> expr )
{
   return look_up_variable(expr->name, expr);
}

std::any Interpreter::visit_AssignExpr(std::shared_ptr<Assign> expr)
{
   std::any value = evaluate(expr->value);
   
   if (locals.find(expr) != locals.end())
   {
      int distance = locals[expr];
      environment->assign_at(distance, expr->name, value);
   }
   else {
      global_environment->assign(expr->name, value);
   }

   return value;  
}

std::any Interpreter::visit_CallExpr(std::shared_ptr<Call> expr)
{
   std::any callee = evaluate(expr->calle);

   std::vector<std::any> arguments;
   for (std::shared_ptr<Expr> argument : expr->arguements)
   {
      arguments.push_back(evaluate(argument));
   }

   std::shared_ptr<LoxCallable> function;
   if (callee.type() == typeid(std::shared_ptr<LoxFunction>)) {
      function = std::any_cast<std::shared_ptr<LoxFunction>>(callee);
   }
   else if (callee.type() == typeid(std::shared_ptr<LoxClass>)) {
      function = std::any_cast<std::shared_ptr<LoxClass>>(callee);
   }
   else {
      throw RuntimeError{expr->paren, "Can only call functions and classes."};
   }

   if (static_cast<int>(arguments.size()) != function->arity()) {
      throw RuntimeError{ expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + "."}; }

   return function->call(*this, std::move(arguments));
}

std::any Interpreter::visit_ExpressionStmt(std::shared_ptr<Expression> stmt)
{
   evaluate(stmt->expression);
   return {};
}

std::any Interpreter::visit_IfStmt(std::shared_ptr<If> stmt)
{
   if (is_truthy(evaluate(stmt->condition)))
   {
      execute(stmt->then_branch);
   }
   else if (stmt->else_branch != nullptr) {
      execute(stmt->else_branch);
   }
   return {};
}

std::any Interpreter::visit_PrintStmt(std::shared_ptr<Print> stmt)
{
   std::any value = evaluate(stmt->expression);
   std::cout << stringify(value) << "\n";
   return {};
}

std::any Interpreter::visit_ReturnStmt(std::shared_ptr<Return> stmt)
{
   std::any value = nullptr;
   if (stmt->value != nullptr) { 
      value = evaluate(stmt->value); 
   }  
   throw LoxReturn{value};
}

std::any Interpreter::visit_VarStmt(std::shared_ptr<Var> stmt)
{
   std::any value = nullptr;
   if (stmt->initializer != nullptr) {
      value = evaluate(stmt->initializer);
   }
   environment->define(stmt->name.lexeme, value);

   return {}; 
}

std::any Interpreter::visit_WhileStmt(std::shared_ptr<While> stmt)
{
   while (is_truthy(evaluate(stmt->condition)))
   {
      execute(stmt->body);
   }

   return {};
}

std::any Interpreter::visit_BlockStmt(std::shared_ptr<Block> stmt)
{
   execute_block(stmt->statements, std::make_shared<Environment>(environment));
   return {};
}

std::any Interpreter::visit_ClassStmt(std::shared_ptr<Class> stmt)
{
   environment->define(stmt->name.lexeme, nullptr);
   auto lox_class = std::make_shared<LoxClass>(stmt->name.lexeme); 
   environment->assign(stmt->name, lox_class);
   return {};
}

std::any Interpreter::visit_FunctionStmt(std::shared_ptr<Function> stmt)
{
   auto function = std::make_shared<LoxFunction>(stmt, environment);
   environment->define(stmt->name.lexeme, function);
   return {};
}

// * All objects that are not nill or false are truthy *
bool Interpreter::is_truthy(std::any object) 
{
   if (object.type() == typeid(nullptr)) { return false; }
   if (object.type() == typeid(bool)) 
   {
      return std::any_cast<bool>(object);
   }
   return true;
}

bool Interpreter::is_equal(std::any a, std::any b)
{
   if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) { return true; }
   if (a.type() == typeid(nullptr)) { return false; }

   if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) 
   {
      return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
   }
   if (a.type() == typeid(double) && b.type() == typeid(double)) 
   {
      return std::any_cast<double>(a) == std::any_cast<double>(b);
   }
   if (a.type() == typeid(bool) && b.type() == typeid(bool)) 
   {
      return std::any_cast<bool>(a) == std::any_cast<bool>(b);
   }
    
   return false;
}

void Interpreter::assert_number_operand(Token op, std::any object)
{
   if ( object.type() == typeid(double) ) { return; }
   else { 
      throw RuntimeError(op, "Operand must be a number."); 
   }
}

void Interpreter::assert_number_operands(Token op, std::any left, std::any right)
{
   if ( left.type() == typeid(double) and right.type() == typeid(double))
   { return; }
   else {
      throw RuntimeError(op, "Operand must be a number."); 
   }
}

std::string Interpreter::stringify(std::any object)
{
   if (object.type() == typeid(nullptr)) { return "nil"; }

   if (object.type() == typeid(double))
   {
      std::string text = std::to_string(std::any_cast<double>(object));
      if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0') {
        text = text.substr(0, text.length() - 2);
      }
   return text;
   }

   if (object.type() == typeid(std::string)) {
      return std::any_cast<std::string>(object);
   }
   
   if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object) ? "true" : "false";
   }

   if (object.type() == typeid(std::shared_ptr<LoxFunction>)) {
      return std::any_cast<
          std::shared_ptr<LoxFunction>>(object)->to_string();
   }
   if (object.type() == typeid(std::shared_ptr<LoxClass>)) {
      return std::any_cast<
         std::shared_ptr<LoxClass>>(object)->to_string();
   }
   if (object.type() == typeid(std::shared_ptr<LoxInstance>)) {
      return std::any_cast<
         std::shared_ptr<LoxInstance>>(object)->to_string();
   }

   return "Error in stringify: object type not recognized.";
}

std::any Interpreter::look_up_variable(Token name, std::shared_ptr<Expr> expr)
{
   if (locals.find(expr) != locals.end())
   {
      int distance = locals[expr];
      return environment->get_at(distance, name.lexeme);
   }
   else {
      return global_environment->get(name);
   }
}