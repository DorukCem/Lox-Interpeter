#pragma once
#include <memory>
#include <vector>
#include "Token.h"

struct Binary;
struct Group;
struct Literal;
struct Unary;
struct Variable;
struct Assign;
struct Logical;
struct Call;

struct ExprVisitor {
  virtual std::any visit_BinaryExpr  (std::shared_ptr<Binary> expr)   = 0;
  virtual std::any visit_GroupExpr   (std::shared_ptr<Group> expr)    = 0;
  virtual std::any visit_LiteralExpr (std::shared_ptr<Literal> expr)  = 0;
  virtual std::any visit_UnaryExpr   (std::shared_ptr<Unary> expr)    = 0;
  virtual std::any visit_VariableExpr(std::shared_ptr<Variable> expr) = 0;
  virtual std::any visit_AssignExpr  (std::shared_ptr<Assign> expr)   = 0;
  virtual std::any visit_LogicalExpr  (std::shared_ptr<Logical> expr) = 0;
  virtual std::any visit_CallExpr (std::shared_ptr<Call> expr)     = 0;
  virtual ~ExprVisitor() = default;
};

struct Expr {
   virtual std::any accept(ExprVisitor& visitor) = 0;
};

/*
   Inherting std::enable_shared_from_this allows the object to return a shared pointer pointing to itself

   These are all tree nodes with slightly different attributes
   For example:
    - Binary node holds an expression to its left and right
               (or)
               /  \
            true  false 
*/ 

struct Binary : Expr, std::enable_shared_from_this<Binary>
{
   const std::shared_ptr<Expr> left;
   const Token op;
   const std::shared_ptr<Expr> right;

   Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

   std::any accept(ExprVisitor &visitor) override;
};

struct Group : Expr, public std::enable_shared_from_this<Group>
{
    const std::shared_ptr<Expr> expr_in;

    explicit Group(std::shared_ptr<Expr> expr);

    std::any accept(ExprVisitor &visitor) override;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal>
{
    const std::any value;

    explicit Literal(std::any value);

    std::any accept(ExprVisitor &visitor) override;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary>
{
    const Token op;
    const std::shared_ptr<Expr> right;

    Unary(Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Variable: Expr, public std::enable_shared_from_this<Variable> {
  Variable(Token name);

  std::any accept(ExprVisitor& visitor) override;

  const Token name;
};

struct Assign: Expr, public std::enable_shared_from_this<Assign> {
  Assign(Token name, std::shared_ptr<Expr> value);

  std::any accept(ExprVisitor& visitor) override;

  const Token name;
  const std::shared_ptr<Expr> value;
};

struct Logical: Expr, public std::enable_shared_from_this<Logical> {
  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);
  
  std::any accept(ExprVisitor& visitor) override;
  
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;
};

struct Call: Expr, public std::enable_shared_from_this<Call> {
  Call(std::shared_ptr<Expr> calle, Token paren, std::vector<std::shared_ptr<Expr>> arguements);

  std::any accept(ExprVisitor& visitor) override;

  const std::shared_ptr<Expr> calle;
  const Token paren;
  const std::vector<std::shared_ptr<Expr>> arguements;
};