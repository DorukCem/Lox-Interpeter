#include "headers/Interpreter.h"

#include "headers/Lox.h"
#include <iostream>

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements)
{
   try 
   {
      for (std::shared_ptr<Stmt> statement : statements) {
         execute(statement);
      }

   } catch (RuntimeError error) {
      Lox::runtime_error(error);
   }
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
   return expr->accept(*this);
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt)
{
   stmt->accept(*this);
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
   return environment.get(expr->name);
}

// ? None of the visit_Stmt's do anything after getting called.
// !! PrintStmt cannot execute after getting called
std::any Interpreter::visit_ExpressionStmt(std::shared_ptr<Expression> stmt)
{
   evaluate(stmt->expression);
   return {};
}

std::any Interpreter::visit_PrintStmt(std::shared_ptr<Print> stmt)
{
   std::any value = evaluate(stmt->expression);
   std::cout << stringify(value) << "\n";
   return {};
}

std::any Interpreter::visit_VarStmt(std::shared_ptr<Var> stmt)
{
   std::any value = nullptr;
   if (stmt->initializer != nullptr) {
      value = evaluate(stmt->initializer);
   }
   environment.define(stmt->name.lexeme, value);

   return {}; 
}

std::any Interpreter::visit_BlockStmt(std::shared_ptr<Block> stmt)
{
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

   return "Error in stringify: object type not recognized.";
}