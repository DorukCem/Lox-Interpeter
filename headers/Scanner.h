#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Token.h"
#include <iostream>

class Scanner {
public:
   Scanner(const std::string& a_source);
   std::vector<Token> scan_tokens();
private:
   static std::unordered_map<std::string, TokenType> keywords;
   const std::string& source;
   std::vector<Token> tokens;
   unsigned int start = 0;
   unsigned int current = 0;
   unsigned int line = 1;
private:
   void scan_token();
   char advance() { return source[current++]; }
   bool is_at_end() { return current >= source.length(); }
   void add_token(TokenType type);
   void add_token(TokenType type, std::any literal);
   bool match(char expected);
   char peek();
   char peek_next();
   void string();
   void number();
   void identifier();
   
};
