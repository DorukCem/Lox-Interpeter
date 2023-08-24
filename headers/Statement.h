#pragma once 

#include "Expr.h"
#include <vector>

struct Block;
struct Expression;
struct Print;
struct Var;

struct StmtVisitor {
  virtual std::any visit_BlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visit_ExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visit_PrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visit_VarStmt(std::shared_ptr<Var> stmt) = 0;
  virtual ~StmtVisitor() = default;
};

struct Stmt {
  virtual std::any accept(StmtVisitor& visitor) = 0;
};

struct Block: Stmt, public std::enable_shared_from_this<Block> {
  Block(std::vector<std::shared_ptr<Stmt>> statements);
  std::any accept(StmtVisitor& visitor) override;
  const std::vector<std::shared_ptr<Stmt>> statements;
};

struct Expression: Stmt, public std::enable_shared_from_this<Expression> {
  Expression(std::shared_ptr<Expr> expression);
  std::any accept(StmtVisitor& visitor) override;
  const std::shared_ptr<Expr> expression;
};

struct Print: Stmt, public std::enable_shared_from_this<Print> {
  Print(std::shared_ptr<Expr> expression);
  std::any accept(StmtVisitor& visitor) override;
  const std::shared_ptr<Expr> expression;
};

struct Var: Stmt, public std::enable_shared_from_this<Var> {
  Var(Token name, std::shared_ptr<Expr> initializer);
  std::any accept(StmtVisitor& visitor) override;
  const Token name;
  const std::shared_ptr<Expr> initializer;
};