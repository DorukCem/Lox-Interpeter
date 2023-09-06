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

While::While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition(condition), body(body)
  {}

std::any While::accept(StmtVisitor& visitor)
{
  return visitor.visit_WhileStmt(shared_from_this());
}

Function::Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body)
  : name(name), params(params), body(body)
{ }

std::any Function::accept(StmtVisitor& visitor)
{
  return visitor.visit_FunctionStmt(shared_from_this());
}

Return::Return(Token keyword, std::shared_ptr<Expr> value)
  : keyword(keyword), value(value)
{ }

std::any Return::accept(StmtVisitor& visitor)
{
  return visitor.visit_ReturnStmt(shared_from_this());
}

Class::Class(Token name, std::vector<std::shared_ptr<Function>> methods)
  : name(name), methods(methods)
{}

std::any Class::accept(StmtVisitor& visitor)
{
  return visitor.visit_ClassStmt(shared_from_this());
}
