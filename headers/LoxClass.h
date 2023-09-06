#pragma once
#include <string>

class LoxClass {
public:
   LoxClass(std::string name) : name(name) {}
   const std::string name;
public:
   const std::string to_string() {
      return name;
   }
};