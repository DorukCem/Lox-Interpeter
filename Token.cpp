#include "headers/Token.h"


Token::Token(TokenType a_type, std::string a_lexeme, std::any a_literal, unsigned int a_line)
:  type(a_type),
   lexeme(a_lexeme),
   literal(a_literal),
   line(a_line)
{ }


std::string token_to_string(TokenType type) {
  static const std::string strings[] = {
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
    "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",
    "BANG", "BANG_EQUAL",
    "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL",
    "LESS", "LESS_EQUAL",
    "IDENTIFIER", "STRING", "NUMBER",
    "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
    "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",
    "END_OF_FILE"
  };

  return strings[static_cast<int>(type)];
}

// ** This Method is only good for debugging
std::string Token::to_string() const
{
  std::string literal_text;

    switch (type) {
      case (IDENTIFIER):
        literal_text = lexeme;
        break;
      case (STRING):
        literal_text = std::any_cast<std::string>(literal);
        break;
      case (NUMBER):
        literal_text = std::to_string(std::any_cast<double>(literal));
        break;
      case (LOX_TRUE):
        literal_text = "true";
        break;
      case (LOX_FALSE):
        literal_text = "false";
        break;
      default:
        literal_text = "nil";
    }

   return token_to_string(type) + " " + lexeme + " " + literal_text;
}