#pragma once

#include <vector>
#include <memory>
#include "Token.h"
#include "Expr.h"

class Parser {
public:
   Parser(std::vector<Token>& tokens);

private:
   const std::vector<Token>& tokens;
   int current = 0;  

private:
   std::shared_ptr<Expr> expression();
   std::shared_ptr<Expr> equality();
   std::shared_ptr<Expr> comparison();
   std::shared_ptr<Expr> term();
   std::shared_ptr<Expr> factor();
   std::shared_ptr<Expr> unary();

   /*
   * This Function can be called with an arbitrary number of 
   * arguments where each argument might be of a different type
   */
   template <typename... T> bool match(T... types);
};
