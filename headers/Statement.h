#pragma once 

#include "Expr.h"
#include <vector>

struct Block;
struct Expression;
struct Print;
struct Var;
struct If;
struct While;
struct Function;
struct Return;
struct Class;

struct StmtVisitor {
  virtual std::any visit_BlockStmt      (std::shared_ptr<Block> stmt)      = 0;
  virtual std::any visit_ExpressionStmt (std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visit_PrintStmt      (std::shared_ptr<Print> stmt)      = 0;
  virtual std::any visit_VarStmt        (std::shared_ptr<Var> stmt)        = 0;
  virtual std::any visit_IfStmt         (std::shared_ptr<If> stmt)         = 0;
  virtual std::any visit_WhileStmt      (std::shared_ptr<While> stmt)      = 0;
  virtual std::any visit_FunctionStmt   (std::shared_ptr<Function> stmt)   = 0;
  virtual std::any visit_ReturnStmt     (std::shared_ptr<Return> stmt)     = 0;
  virtual std::any visit_ClassStmt      (std::shared_ptr<Class> stmt)      = 0;
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

struct If: Stmt, public std::enable_shared_from_this<If> {
  If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch);
  std::any accept(StmtVisitor& visitor) override;
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> then_branch;
  const std::shared_ptr<Stmt> else_branch;
};

struct While: Stmt, public std::enable_shared_from_this<While> {
  While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);
  std::any accept(StmtVisitor& visitor) override;
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> body;
};

struct Function: Stmt, public std::enable_shared_from_this<Function> {
  Function( Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body);
  std::any accept(StmtVisitor& visitor) override;
  const Token name;
  const std::vector<Token> params;
  const std::vector<std::shared_ptr<Stmt>> body;
};

struct Return: Stmt, public std::enable_shared_from_this<Return> {
  Return(Token keyword, std::shared_ptr<Expr> value);
  std::any accept(StmtVisitor& visitor) override;
  const Token keyword;
  const std::shared_ptr<Expr> value;  
};

struct Class: Stmt, public std::enable_shared_from_this<Class> {
  Class(Token name, std::shared_ptr<Variable> superclass, std::vector<std::shared_ptr<Function>> methods);
  std::any accept(StmtVisitor& visitor) override;
  const Token name;
  const std::shared_ptr<Variable> superclass;
  const std::vector<std::shared_ptr<Function>> methods;
};
