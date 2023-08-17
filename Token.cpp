#include "headers/Token.h"


Token::Token(TokenType a_type, const std::string& a_lexeme, const std::string& a_literal, unsigned int a_line)
:  type(a_type),
   lexeme(a_lexeme),
   literal(a_literal),
   line(a_line)
{ }

// ** This Method is only good for debugging
std::string Token::to_string() const
{
   // for string and number literals, use actual value
   if (type == TokenType::STRING || type == TokenType::NUMBER) {
      return "literal: " + literal;
   }
   if (type == TokenType::IDENTIFIER)
   {
      return "var: " + lexeme;
   }
   return "keyword: "+ lexeme;
}