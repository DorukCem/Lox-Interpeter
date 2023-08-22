#pragma once
#include "Expr.h"

class Interpeter : ExprVisitor {
public:
   std::any visit_BinaryExpr (std::shared_ptr<Binary> expr)  override;
   std::any visit_GroupExpr  (std::shared_ptr<Group> expr)   override;
   std::any visit_LiteralExpr(std::shared_ptr<Literal> expr) override;
   std::any visit_UnaryExpr  (std::shared_ptr<Unary> expr)   override;

private:
   std::any evaluate(std::shared_ptr<Expr> expr);
   bool is_truthy(std::any object);
   bool is_equal(std::any a, std::any b);
   void assert_number_operand(Token op, std::any object);
   void assert_number_operands(Token op, std::any left, std::any right);
};
