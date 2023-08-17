#pragma once
#include <string>

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
   const std::string lexeme;
   const std::string literal;
   const int line;   

   public:
   Token(TokenType a_type, const std::string& a_lexeme, const std::string& a_literal, unsigned int a_line);
   std::string to_string() const;
};





