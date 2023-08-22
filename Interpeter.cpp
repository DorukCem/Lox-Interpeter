#include "headers/Interpeter.h"
#include "headers/Error.h"




std::any Interpeter::evaluate(std::shared_ptr<Expr> expr)
{
   return expr->accept(*this);
}

std::any Interpeter::visit_BinaryExpr(std::shared_ptr<Binary> expr)
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

std::any Interpeter::visit_GroupExpr(std::shared_ptr<Group> expr)
{
   return evaluate(expr->expr_in);
}

std::any Interpeter::visit_LiteralExpr(std::shared_ptr<Literal> expr)
{
   return expr->value;
}

std::any Interpeter::visit_UnaryExpr(std::shared_ptr<Unary> expr)
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

// * All objects that are not nill or false are truthy *
bool Interpeter::is_truthy(std::any object) 
{
   if (object.type() == typeid(nullptr)) { return false; }
   if (object.type() == typeid(bool)) 
   {
      return std::any_cast<bool>(object);
   }
   return true;
}

bool Interpeter::is_equal(std::any a, std::any b)
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

void Interpeter::assert_number_operand(Token op, std::any object)
{
   if ( object.type() == typeid(double) ) { return; }
   else { 
      throw RuntimeError(op, "Operand must be a number."); 
   }
}

void Interpeter::assert_number_operands(Token op, std::any left, std::any right)
{
   if ( left.type() == typeid(double) and right.type() == typeid(double))
   { return; }
   else {
      throw RuntimeError(op, "Operand must be a number."); 
   }
}