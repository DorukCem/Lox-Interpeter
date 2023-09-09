#include "headers/Expr.h"

// * The visitor has a differet method for each class
// * When a Visitor "visits" an Expr 
// * The Expr calls the method that is asssociated with itself and passes a pointer pointing to itself
// ** -- For example : 
// ** --    Visitor visits a BinaryExpr by calling its accept method and passing itself by reference
// ** --    the BinaryExpr calls the visit_BinaryExpr method of the visitor and passes a pointer to itself as an argument  

Binary::Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
   : left(left), op(op), right(right)
{ }

std::any Binary::accept(ExprVisitor &visitor) 
{
   return visitor.visit_BinaryExpr(shared_from_this());
}
// *----------------Group----------------------
Group::Group(std::shared_ptr<Expr> expr_in) 
   : expr_in(expr_in)
{ }

std::any Group::accept(ExprVisitor &visitor)
{
   return visitor.visit_GroupExpr(shared_from_this());
}

// *----------------Literal----------------------

Literal::Literal(std::any value) 
   : value(value)
{ }

std::any Literal::accept(ExprVisitor &visitor)
{
   return visitor.visit_LiteralExpr(shared_from_this());
}

// *-----------------Unary-----------------------

Unary::Unary(Token op, std::shared_ptr<Expr> right) 
   : op(op), right(right)
{ }

std::any Unary::accept(ExprVisitor &visitor)
{
   return visitor.visit_UnaryExpr(shared_from_this());
}

// *-----------------Variable-----------------------

Variable::Variable(Token name)
    : name(name)
  {}

std::any Variable::accept(ExprVisitor& visitor) {
   return visitor.visit_VariableExpr(shared_from_this());
}

// *-----------------Assign-----------------------

Assign::Assign(Token name, std::shared_ptr<Expr> value)
   : name(name), value(value)
{ }


std::any Assign::accept(ExprVisitor& visitor)
{
   return visitor.visit_AssignExpr(shared_from_this());
}

// *-----------------Logical-----------------------

Logical::Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
   :left(left), op(op), right(right)
{ }

std::any Logical::accept(ExprVisitor& visitor)
{
  return visitor.visit_LogicalExpr(shared_from_this());
}

// *-----------------Call-----------------------

Call::Call(std::shared_ptr<Expr> calle, Token paren, std::vector<std::shared_ptr<Expr>> arguements)
   :calle(calle), paren(paren), arguements(arguements)
{}

std::any Call::accept(ExprVisitor& visitor)
{
   return visitor.visit_CallExpr(shared_from_this());
}

// *-----------------Get-----------------------

Get::Get(std::shared_ptr<Expr> object, Token name)
   :object(object), name(name)
{}


std::any Get::accept(ExprVisitor& visitor)
{
   return visitor.visit_GetExpr(shared_from_this());
}

// *-----------------Set-----------------------

Set::Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
   :object(object), name(name), value(value)
{ }

std::any Set::accept(ExprVisitor& visitor)
{
   return visitor.visit_SetExpr(shared_from_this());
}

// *-----------------This-----------------------

This::This(Token keyword)
   : keyword(keyword)
{ }

std::any This::accept(ExprVisitor& visitor)
{
   return visitor.visit_ThisExpr(shared_from_this());
}

// *-----------------Super-----------------------

Super::Super(Token keyword, Token method)
   : keyword(keyword), method(method)
{ }

std::any Super::accept(ExprVisitor& visitor)
{
   return visitor.visit_SuperExpr(shared_from_this());
}
