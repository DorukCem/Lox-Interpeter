#include "headers/Parser.h"
#include "headers/Lox.h"
#include <cassert>

// ? For some reason test.lox has to start with a literal

Parser::Parser(std::vector<Token>& tokens)
   :tokens(tokens)
{ 
}

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
   std::vector<std::shared_ptr<Stmt>> statements;
   while (!is_at_end()) {
      statements.push_back( declaration() );
   }

    return statements; 
}

std::shared_ptr<Expr> Parser::expression()
{
   return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
   std::shared_ptr<Expr> expr = comparison();

   while ( match( BANG_EQUAL, EQUAL_EQUAL ))
   {
      Token op = previous();
      std::shared_ptr<Expr> right = comparison();
      expr = std::make_shared<Binary>(expr, op, right);
   }

   return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
   std::shared_ptr<Expr> expr = term();

   while ( match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL) )
   {
      Token op = previous();
      std::shared_ptr<Expr> right = term();
      expr = std::make_shared<Binary>(expr, op, right);
   }

   return expr;
}

std::shared_ptr<Expr> Parser::term()
{
   std::shared_ptr<Expr> expr = factor();

   while (match(MINUS, PLUS)) 
   {
      Token op = previous();
      std::shared_ptr<Expr> right = factor();
      expr = std::make_shared<Binary>(expr, op, right);
   }

   return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
   std::shared_ptr<Expr> expr = unary();

   while (match(SLASH, STAR)) 
   {
      Token op = previous();
      std::shared_ptr<Expr> right = unary();
      expr = std::make_shared<Binary>(expr, op, right);
   }

   return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
   if (match(BANG, MINUS)) 
   {
      Token op = previous();
      std::shared_ptr<Expr> right = unary();
      return std::make_shared<Unary>(op, right);
   }

   return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
   if (match(LOX_FALSE)) return std::make_shared<Literal>(false);
   if (match(LOX_TRUE)) return std::make_shared<Literal>(true);
   if (match(NIL)) return std::make_shared<Literal>(nullptr);

   if (match(NUMBER, STRING)) {
      return std::make_shared<Literal>(previous().literal);
   }

   if (match(IDENTIFIER)) {
      return std::make_shared<Variable>(previous());
   }

   if (match(LEFT_PAREN)) {
      std::shared_ptr<Expr> expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_shared<Group>(expr);
   }

   throw error(peek(), "Expect expression.");
}


/*
   * This Function can be called with an arbitrary number of 
   * arguments where each argument might be of a different type
*/
template<typename... T>
bool Parser::match(T... types)
{  
   assert((... && std::is_same_v<T, TokenType>));
   // iterate over the argumentsand apply the check function to each of them
   // then combine the results of the check function by OR'ing them 
   if ((... || check(types))) {
      advance();
      return true;
   }

   return false;  
}

bool Parser::check(TokenType type)
{
   if (is_at_end()) { return false; }
   return peek().type == type;
}

Token Parser::advance()
{
   if (not is_at_end()) { current++; }
   return previous();
}

bool Parser::is_at_end()
{
   return peek().type == END_OF_FILE;
}

Token Parser::peek()
{
   return tokens.at(current);
}

Token Parser::previous()
{
   return tokens.at(current - 1);
}

Token Parser::consume(TokenType type, std::string message)
{
   if (check(type)) { return advance(); }

   throw error(peek(), message);
}

ParseError Parser::error(Token token, std::string message)
{
   Lox::error(token, message);
   return ParseError("");
}

/*
   We still need to continue parsing to find aditional errors. 
   To do that we must find the point where the next statement begins
   We want to discard tokens until we’re right at the beginning of the next statement.
*/
void Parser::synchronize()
{
   advance();

    while (not is_at_end()) {
      if (previous().type == SEMICOLON) return;

      switch (peek().type) {
         case CLASS:
         case FUN:
         case VAR:
         case FOR:
         case IF:
         case WHILE:
         case PRINT:
         case RETURN:
            return;
         
         default: ;// Do nothing
      }

      advance();
   }
}

std::shared_ptr<Stmt> Parser::statement()
{
   if (match(PRINT)){ return print_statement(); }

   return expression_statement();
}

std::shared_ptr<Stmt> Parser::print_statement()
{
   std::shared_ptr<Expr> value = expression();
   consume(SEMICOLON, "Expect ';' after value.");
   return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::expression_statement()
{
   std::shared_ptr<Expr> expr = expression();
   consume(SEMICOLON, "Expect ';' after expression.");
   return std::make_shared<Expression>(expr);
}

std::shared_ptr<Stmt> Parser::declaration()
{
   try 
   {
      if (match(VAR)) { return var_declaration(); }
      return statement();
   } 
   catch (ParseError error) {
      synchronize(); // We can synchronize after error in case we find a variable declartion
      return nullptr; 
   }
}

std::shared_ptr<Stmt> Parser::var_declaration()
{
   Token name = consume(IDENTIFIER, "Expect variable name.");

   std::shared_ptr<Expr> initializer = nullptr;
   if (match(EQUAL)) {
      initializer = expression();
   }
   consume(SEMICOLON, "Expect ';' after variable declaration.");
   return std::make_shared<Var>(Var(name, initializer));
}