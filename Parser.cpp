#include "headers/Parser.h"
#include "headers/Lox.h"
#include <cassert>
#include <iostream>

Parser::Parser(std::vector<Token>& tokens)
   :tokens(tokens)
{ 
}

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
   std::vector<std::shared_ptr<Stmt>> statements;
   while (!is_at_end()) {
      statements.push_back( declaration() ); // ! I want to replace this with a function like recursive_decent() 
   }
   
   return statements; 
}

std::shared_ptr<Stmt> Parser::declaration()
{
   try 
   {
      if (match(CLASS)) { return class_declaration(); }
      if (match(FUN)) { return function("function"); }
      if (match(VAR)) { return var_declaration(); }
      return statement();
   } 
   catch (ParseError const& error) { //! catch error by reference??
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

std::shared_ptr<Stmt> Parser::class_declaration()
{
   Token name = consume(IDENTIFIER, "Expect class name.");

   std::shared_ptr<Variable> superclass = nullptr;
   if (match(LESS)) {
      consume(IDENTIFIER, "Expect superclass name.");
      superclass = std::make_shared<Variable>(previous());  
   }
   consume(LEFT_BRACE, "Expect '{' before class body.");
   std::vector<std::shared_ptr<Function>> methods;
   while( !check(RIGHT_BRACE) and !is_at_end()) {
      methods.push_back( function("method") );
   }

   consume(RIGHT_BRACE, "Expect '}' after class body.");

   return std::make_shared<Class>(name, superclass, methods);
}

std::shared_ptr<Expr> Parser::expression()
{
   return assignment();
}
 
std::shared_ptr<Expr> Parser::assignment()
{
   std::shared_ptr<Expr> expr = or_expression();

   if (match(EQUAL)) 
   {
      Token equals = previous();
      std::shared_ptr<Expr> value = assignment();  // ** <- A recursive call

      // * https://stackoverflow.com/questions/19501838/get-derived-type-via-base-class-virtual-function
      // ** Check if expr is of type Variable or Get
      if (Variable* e = dynamic_cast<Variable*>(expr.get())) 
      {
         Token name = e->name;
         return std::make_shared<Assign>(name, value);
      }
      else if (Get* get = dynamic_cast<Get*>(expr.get())) 
      {
         return std::make_shared<Set>(get->object, get->name, value);
      }
      error(equals, "Invalid assignment target.");
   }

   return expr;
}

std::shared_ptr<Expr> Parser::or_expression()
{
   std::shared_ptr<Expr> expr = and_expression();

   while (match(OR))
   {
      Token op = previous();
      std::shared_ptr<Expr> right = equality();
      expr = std::make_shared<Logical>(expr, op, right);
   }
   
   return expr;
}

std::shared_ptr<Expr> Parser::and_expression()
{
   std::shared_ptr<Expr> expr = equality();

   while (match(AND))
   {
      Token op = previous();
      std::shared_ptr<Expr> right = equality();
      expr = std::make_shared<Logical>(expr, op, right);
   }
   
   return expr;
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

   return call();
}

std::shared_ptr<Expr> Parser::call()
{
   std::shared_ptr<Expr> expr = primary();

   while (true) 
   {
      if (match(LEFT_PAREN))
      {
         expr = finish_call(expr);
      } 
      else if (match(DOT)) 
      {
         Token name = consume(IDENTIFIER, "Expect property name after '.'.");
         expr = std::make_shared<Get>(expr, name);
      }
      else { break; }
   }

   return expr;
}

std::shared_ptr<Expr> Parser::finish_call(std::shared_ptr<Expr> callee)
{
   std::vector<std::shared_ptr<Expr>> arguments;

   if (!check(RIGHT_PAREN))
   {
      do { 
         if (arguments.size() >= 255) { error(peek(), "Can't have more than 255 arguments."); }
         arguments.push_back(expression()); 
      }
      while ( match(COMMA) );
   }

   Token paren = consume(RIGHT_PAREN, "Expect ')' after arguements.");

   return std::make_shared<Call>(callee, paren, arguments);
}

// ** this is the last stop of recursion
std::shared_ptr<Expr> Parser::primary()
{
   if (match(LOX_FALSE)) {return std::make_shared<Literal>(false);}
   if (match(LOX_TRUE)) {return std::make_shared<Literal>(true);}
   if (match(NIL)) {return std::make_shared<Literal>(nullptr);}

   if (match(NUMBER, STRING)) {
      return std::make_shared<Literal>(previous().literal);
   }
   if (match(SUPER)) {
      Token keyword = previous();
      consume(DOT, "Expect '.' after 'super'.");
      Token method = consume(IDENTIFIER, "Expect superclass method name.");
      return std::make_shared<Super>(keyword, method);
   }

   if (match(THIS)) {return std::make_shared<This>(previous());}

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

std::shared_ptr<Stmt> Parser::statement()
{
   if (match(PRINT)){ return print_statement(); }
   if (match(RETURN)){ return return_statement(); }
   if (match(WHILE)){ return while_statement(); }
   if (match(LEFT_BRACE)) { return std::make_shared<Block>(block()); } // -> Initilize a shared pointer to a block object with a vector of statements that is returned by the block() function
   if (match(FOR)) { return for_statement(); }
   if (match(IF)) { return if_statement(); }
   return expression_statement();
} 

/*
   The sytax is:
   for (var i = 0; i < 10; i = i + 1)
   {
      do_something()
   }
   
   The for statement gets converted into a while statement
*/
std::shared_ptr<Stmt> Parser::for_statement()
{
   consume(LEFT_PAREN, "Expect '(' after 'for'.");

   std::shared_ptr<Stmt> initializer;
   if (match(SEMICOLON)) 
   {
      initializer = nullptr;
   } 
   else if (match(VAR)) 
   {
      initializer = var_declaration();
   } 
   else 
   {
      initializer = expression_statement();
   }

   std::shared_ptr<Expr> condition = nullptr;
   if ( !check(SEMICOLON) ) 
   {
      condition = expression();
   }
   consume(SEMICOLON, "Expect ';' after loop condition.");

   std::shared_ptr<Expr> increment = nullptr;
   if (!check(RIGHT_PAREN)) 
   {
      increment = expression();
   }
   consume(RIGHT_PAREN, "Expect ')' after for clauses.");

   std::shared_ptr<Stmt> body = statement();

   if (increment != nullptr)
   {
      body = std::make_shared<Block>( std::vector<std::shared_ptr<Stmt>> { body, std::make_shared<Expression>(increment) } );
   }

   if (condition == nullptr) { condition = std::make_shared<Literal>(true); }
   body = std::make_shared<While>(condition, body);

   if (initializer != nullptr) {
      body = std::make_shared<Block>(std::vector<std::shared_ptr<Stmt>> {initializer, body} );
   }

   return body;
}

std::shared_ptr<Stmt> Parser::if_statement()
{
   consume(LEFT_PAREN, "Expect '(' after 'if'.");
   std::shared_ptr<Expr> condition = expression();
   consume(RIGHT_PAREN, "Expect ')' after 'if condition'.");

   std::shared_ptr<Stmt> then_branch = statement();
   std::shared_ptr<Stmt> else_branch = nullptr;
   if (match(ELSE)) {
      else_branch = statement();
   }
   
   return std::make_shared<If>(condition, then_branch, else_branch);
}

std::shared_ptr<Stmt> Parser::while_statement()
{
   consume(LEFT_PAREN, "Expect '(' after 'while'.");
   std::shared_ptr<Expr> condition = expression();
   consume(RIGHT_PAREN, "Expect ')' after condition.");
   std::shared_ptr<Stmt> body = statement();

   return std::make_shared<While>(condition, body);
}

std::shared_ptr<Stmt> Parser::print_statement()
{
   std::shared_ptr<Expr> value = expression();
   consume(SEMICOLON, "Expect ';' after value.");
   return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::return_statement()
{
   Token keyword = previous();
   std::shared_ptr<Expr> value = nullptr;
   if (!check(SEMICOLON)) {
      value = expression();
   }

   consume(SEMICOLON, "Expect ';' after return value.");
   return std::make_shared<Return>(keyword, value);
}

std::shared_ptr<Stmt> Parser::expression_statement()
{
   std::shared_ptr<Expr> expr = expression();
   consume(SEMICOLON, "Expect ';' after expression.");
   return std::make_shared<Expression>(expr);
}

std::shared_ptr<Function> Parser::function(std::string kind)
{
   Token name = consume(IDENTIFIER,  "Expect " + kind + " name.");
   consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
   std::vector<Token> parameters;
   if (!check(RIGHT_PAREN))
   {
      do {
         if (parameters.size() >= 255) {
            error(peek(), "Can't have more than 255 parameters.");
         }
         parameters.push_back( consume(IDENTIFIER, "Expect parameter name.") );
      } while (match(COMMA));
   }
   consume(RIGHT_PAREN, "Expect ')' after parameters.");

   consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
   std::vector<std::shared_ptr<Stmt>> body = block();
   return std::make_shared<Function>(name, parameters, body);
}

std::vector<std::shared_ptr<Stmt>> Parser::block()
{
   std::vector<std::shared_ptr<Stmt>> statements;

   while( !check(RIGHT_BRACE) and !is_at_end() )
   {
      statements.push_back(declaration());
   }

   consume(RIGHT_BRACE, "Expect '}' after block.");
   return statements;
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