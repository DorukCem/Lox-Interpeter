#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "Expr.h"
#include "Statement.h"

struct ParseError : public std::runtime_error 
{
   using std::runtime_error::runtime_error; // Use the constructor of runtime error without any changes
};

class Parser {
public:
   Parser(std::vector<Token>& tokens);
   std::vector<std::shared_ptr<Stmt>> parse();

private:
   const std::vector<Token>& tokens;
   int current = 0;  

private:
   std::shared_ptr<Expr> expression();
   std::shared_ptr<Expr> or_expression();
   std::shared_ptr<Expr> and_expression();
   std::shared_ptr<Expr> equality();
   std::shared_ptr<Expr> comparison();
   std::shared_ptr<Expr> assignment();
   std::shared_ptr<Expr> term();
   std::shared_ptr<Expr> factor();
   std::shared_ptr<Expr> unary();
   std::shared_ptr<Expr> call();
   std::shared_ptr<Expr> finish_call(std::shared_ptr<Expr> callee);
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
   std::shared_ptr<Stmt> statement();
   std::shared_ptr<Stmt> for_statement();
   std::shared_ptr<Stmt> if_statement();
   std::shared_ptr<Stmt> while_statement();
   std::shared_ptr<Stmt> print_statement();
   std::shared_ptr<Stmt> return_statement();
   std::shared_ptr<Stmt> expression_statement();
   std::shared_ptr<Stmt> declaration();
   std::shared_ptr<Stmt> var_declaration();
   std::shared_ptr<Stmt> class_declaration();
   std::shared_ptr<Function> function(std::string kind);
   std::vector<std::shared_ptr<Stmt>> block();

};
