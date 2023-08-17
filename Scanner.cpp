#include "headers/Scanner.h"
#include "headers/Lox.h"
#include <iostream>

std::unordered_map<std::string, TokenType> Scanner::keywords = {
   {"and",    AND},
   {"class",  CLASS},
   {"else",   ELSE},
   {"false",  LOX_FALSE},
   {"for",    FOR},
   {"fun",    FUN},
   {"if",     IF},
   {"nil",    NIL},
   {"or",     OR},
   {"print",  PRINT},
   {"return", RETURN},
   {"super",  SUPER},
   {"this",   THIS},
   {"true",   LOX_TRUE},
   {"var",    VAR},
   {"while",  WHILE},
};


Scanner::Scanner(const std::string& a_source)
:  source(a_source)
{  }

void Scanner::add_token(TokenType type)
{
   add_token(type, "");   
}

void Scanner::add_token(TokenType type, std::string literal)
{
   std::string text = source.substr(start, current - start);
   tokens.push_back(Token(type, text, literal, line));
}

char Scanner::peek()
{
   if (is_at_end()) { return '\0'; }
   return source[current];
}

char Scanner::peek_next()
{
   if (current + 1 >= source.length()) { return '\0'; }
   return source[current + 1];
}

bool Scanner::match(char expected)
{
   if (is_at_end()) { return false; }
   if (source[current] != expected) { return false; }
   current++;
   return true;
}

std::vector<Token> Scanner::scan_tokens()
{
   while (not is_at_end())
   {
      start = current;
      scan_token();
   }
   tokens.push_back(Token(END_OF_FILE, "end", "", line));
   return tokens;
}

void Scanner::scan_token()
{
   char c = advance();
    switch (c) {
      case '(': add_token(LEFT_PAREN); break;
      case ')': add_token(RIGHT_PAREN); break;
      case '{': add_token(LEFT_BRACE); break;
      case '}': add_token(RIGHT_BRACE); break;
      case ',': add_token(COMMA); break;
      case '.': add_token(DOT); break;
      case '-': add_token(MINUS); break;
      case '+': add_token(PLUS); break;
      case ';': add_token(SEMICOLON); break;
      case '*': add_token(STAR); break; 
      case '!':
        add_token(match('=') ? BANG_EQUAL : BANG);
        break;
      case '=':
        add_token(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
      case '<':
        add_token(match('=') ? LESS_EQUAL : LESS);
        break;
      case '>':
        add_token(match('=') ? GREATER_EQUAL : GREATER);
        break;

      case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !is_at_end()) 
               { advance(); }
        } 
        else {
            add_token(SLASH);
        }
        break;

      // Ignore whitespace.
      case ' ':
      case '\r':
      case '\t':
         break;

      case '\n':
         line++;
         break;

      case '"': string(); break;

      default:
         if (isdigit(c)) { number(); }
         else if (isalpha(c)) {
            identifier();
         }
         else {
            Lox::error(line, "Unexpected character: " + c);
            break;
         }
    }
}

void Scanner::string()
{
   while (peek() != '"' && !is_at_end()) {
      if (peek() == '\n') { 
         line++; 
      }
      advance();
   }

   if (is_at_end()) {
      Lox::error(line, "Unterminated string.");
      return;
   }

   // The closing ".
   advance();
   // Trim the surrounding quotes.
   std::string value = source.substr(start + 1, current - 2);
   add_token(STRING, value);
}

void Scanner::number()
{
   while (isdigit( peek() )) {
      advance();
   }
    // Look for a fractional part.
   if (peek() == '.' && isdigit( peek_next() )) 
   {
      // Consume the "."
      advance();

      while (isdigit( peek() )) {
         advance();
      }
   }

// ! double is a string
   add_token(NUMBER, source.substr(start, current-start) );
}

void Scanner::identifier()
{
   while (isalnum(peek())) {
      advance();
   }
   std::string text = source.substr(start, current-start);   
   auto is_in_map = keywords.find(text);
   TokenType type = (is_in_map == keywords.end()) ? IDENTIFIER : keywords[text];
   
   add_token(type);
}