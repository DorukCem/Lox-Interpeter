#include "headers/Parser.h"

Parser::Parser(std::vector<Token>& tokens)
   :tokens(tokens)
{ 
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
}

std::shared_ptr<Expr> Parser::comparison()
{
   
}

std::shared_ptr<Expr> Parser::term()
{
   
}

std::shared_ptr<Expr> Parser::factor()
{
   
}

std::shared_ptr<Expr> Parser::unary()
{
   
}


template<typename... T>
bool Parser::match(T... types)
{  
   // iterate over the argumentsand apply the check function to each of them
   // then combine the results of the check function by OR'ing them 
   if ((... || check(type))) {
      advance();
      return true;
   }

   return false;  
}