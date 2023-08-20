#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "Expr.h"

struct ParseError : public std::runtime_error 
{
   using std::runtime_error::runtime_error; // Use the constructor of runtime error without any changes
};

class Parser {
public:
   Parser(std::vector<Token>& tokens);
   std::shared_ptr<Expr> parse();

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
   std::shared_ptr<Expr> primary();

   template <typename... T> bool match(T... types);
   bool check(TokenType type);
   bool is_at_end();
   Token advance();
   Token peek();
   Token previous();
   Token consume(TokenType type, std::string message);
   ParseError error(Token token, std::string message);
   void synchronize();
};