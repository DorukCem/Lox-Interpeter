#pragma once
#include "Expr.h"
#include "Statement.h"
#include "Interpreter.h"
#include "map"

enum class FunctionType {
   NONE,
   FUNCTION
};

class Resolver : ExprVisitor, StmtVisitor {
public:
   Resolver(Interpreter& interpreter);
   std::any visit_BlockStmt     (std::shared_ptr<Block> stmt)      override;
   std::any visit_VarStmt       (std::shared_ptr<Var> stmt)        override;
   std::any visit_ExpressionStmt(std::shared_ptr<Expression> stmt) override;
   std::any visit_IfStmt(std::shared_ptr<If> stmt)         override;
   std::any visit_PrintStmt(std::shared_ptr<Print> stmt)      override;
   std::any visit_ReturnStmt(std::shared_ptr<Return> stmt)    override;
   std::any visit_WhileStmt(std::shared_ptr<While> stmt)     override;
   std::any visit_FunctionStmt(std::shared_ptr<Function> stmt)   override;
   std::any visit_ClassStmt(std::shared_ptr<Class> stmt) override;
   std::any visit_VariableExpr(std::shared_ptr<Variable> expr)   override;
   std::any visit_AssignExpr(std::shared_ptr<Assign> expr)   override;
   std::any visit_BinaryExpr(std::shared_ptr<Binary> expr)       override;
   std::any visit_CallExpr(std::shared_ptr<Call> expr)       override;
   std::any visit_GroupExpr(std::shared_ptr<Group> expr)       override;
   std::any visit_LiteralExpr(std::shared_ptr<Literal> expr)       override;
   std::any visit_LogicalExpr(std::shared_ptr<Logical> expr)       override;
   std::any visit_UnaryExpr(std::shared_ptr<Unary> expr)       override;

   void resolve(std::vector<std::shared_ptr<Stmt>> statements);
private:
   Interpreter& interpreter;
   std::vector<std::map<std::string, bool>> scopes;
   FunctionType current_function = FunctionType::NONE;
private:
   void resolve(std::shared_ptr<Stmt> stmt);
   void resolve(std::shared_ptr<Expr> expr);
   void begin_scope();
   void end_scope();
   void declare(Token name);
   void define(Token name);
   void resolve_local(std::shared_ptr<Expr> expr, Token name);
   void resolve_function(std::shared_ptr<Function> function, FunctionType type); 
};
