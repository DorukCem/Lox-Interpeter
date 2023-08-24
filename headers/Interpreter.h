#pragma once
#include "Expr.h"
#include "Statement.h"

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
   std::any visit_BinaryExpr (std::shared_ptr<Binary> expr)  override;
   std::any visit_GroupExpr  (std::shared_ptr<Group> expr)   override;
   std::any visit_LiteralExpr(std::shared_ptr<Literal> expr) override;
   std::any visit_UnaryExpr  (std::shared_ptr<Unary> expr)   override;
   std::any visit_ExpressionStmt (std::shared_ptr<Expression> stmt) override;
   std::any visit_PrintStmt      (std::shared_ptr<Print> stmt) override;
   std::any visit_VarStmt        (std::shared_ptr<Var> stmt) override;
   std::any visit_BlockStmt      (std::shared_ptr<Block> stmt) override;
   void interpret(std::vector<std::shared_ptr<Stmt>> staments);

private:
   std::any evaluate(std::shared_ptr<Expr> expr);
   void execute(std::shared_ptr<Stmt> stmt);
   bool is_truthy(std::any object);
   bool is_equal(std::any a, std::any b);
   void assert_number_operand(Token op, std::any object);
   void assert_number_operands(Token op, std::any left, std::any right);
   std::string stringify(std::any object);
};


