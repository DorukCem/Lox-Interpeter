#pragma once
#include <string>
#include <any>

enum TokenType
{
   // Single-character tokens.
   LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
   COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

   // One or two character tokens.
   BANG, BANG_EQUAL,
   EQUAL, EQUAL_EQUAL,
   GREATER, GREATER_EQUAL,
   LESS, LESS_EQUAL,

   // Literals.
   IDENTIFIER, STRING, NUMBER,

   // Keywords.
   AND, CLASS, ELSE, LOX_FALSE, FUN, FOR, IF, NIL, OR,
   PRINT, RETURN, SUPER, THIS, LOX_TRUE, VAR, WHILE,

   END_OF_FILE
};

class Token
{
   const TokenType type;       
   std::string lexeme; // The group of characters
   std::any literal; // The actual value that the token is holding, if any 
   const int line;            // For better errror reporting

   public:
   Token(TokenType a_type, std::string a_lexeme, std::any a_literal, unsigned int a_line);
   std::string to_string() const;
};


// * Each group of charachters is called a lexeme 
