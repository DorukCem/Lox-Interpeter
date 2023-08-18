#pragma once
#include <memory>
#include "Token.h"


struct Binary;
struct Group;
struct Literal;
struct Unary;

struct ExprVisitor {
  virtual std::any visit_BinaryExpr(std::shared_ptr<Binary> expr) = 0;
  virtual std::any visit_GroupExpr(std::shared_ptr<Group> expr) = 0;
  virtual std::any visit_LiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visit_UnaryExpr(std::shared_ptr<Unary> expr) = 0;
  virtual ~ExprVisitor() = default;
};

struct Expr {
   virtual std::any accept(ExprVisitor& visitor) = 0;
};

/*
   Inherting std::enable_shared_from_this allows the object to return a shared pointer pointing to itself
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