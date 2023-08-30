#include "headers/Statement.h"
#include "iostream"

Block::Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements{statements}
  {}

std::any Block::accept(StmtVisitor& visitor) {
    return visitor.visit_BlockStmt(shared_from_this());
  }

Expression::Expression(std::shared_ptr<Expr> expression)
    : expression{expression}
  {}

std::any Expression::accept(StmtVisitor& visitor) {
    return visitor.visit_ExpressionStmt(shared_from_this());
  }

Print::Print(std::shared_ptr<Expr> expression)
    : expression{expression}
  {}

std::any Print::accept(StmtVisitor& visitor) {
    return visitor.visit_PrintStmt(shared_from_this());
  }

Var::Var(Token name, std::shared_ptr<Expr> initializer)
    : name{name}, initializer{initializer}
  {}

std::any Var::accept(StmtVisitor& visitor) {
    return visitor.visit_VarStmt(shared_from_this());
  }

If::If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch)
    : condition(condition), then_branch(then_branch), else_branch(else_branch) 
 {}

std::any If::accept(StmtVisitor& visitor){
  return visitor.visit_IfStmt(shared_from_this()); 
}