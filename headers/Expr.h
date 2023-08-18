#pragma once
#include <memory>
#include "headers/Token.h"

struct Expr {
   // put pure virtual function here
};


// Inherting std::enable_shared_from_this allows the object to return a shared pointer pointing to itself
struct Binary : Expr, std::enable_shared_from_this<Binary>
{
   const std::shared_ptr<Expr> left;
   const Token op;
   const std::shared_ptr<Expr> right;

   Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
   : left(left), op(op), right(right)
   {}
};