#pragma once
#include "Expr.h"
#include "Statement.h"
#include "Environment.h"
#include "LoxCallable.h"
#include <chrono>

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
   std::any visit_BinaryExpr  (std::shared_ptr<Binary> expr)   override;
   std::any visit_GroupExpr   (std::shared_ptr<Group> expr)    override;
   std::any visit_LiteralExpr (std::shared_ptr<Literal> expr)  override;
   std::any visit_UnaryExpr   (std::shared_ptr<Unary> expr)    override;
   std::any visit_VariableExpr(std::shared_ptr<Variable> expr) override;
   std::any visit_AssignExpr  (std::shared_ptr<Assign> expr)   override; 
   std::any visit_LogicalExpr (std::shared_ptr<Logical> expr)  override; 
   std::any visit_CallExpr    (std::shared_ptr<Call> expr)     override; 
   std::any visit_ExpressionStmt (std::shared_ptr<Expression> stmt) override;
   std::any visit_PrintStmt      (std::shared_ptr<Print> stmt)      override;
   std::any visit_VarStmt        (std::shared_ptr<Var> stmt)        override;
   std::any visit_BlockStmt      (std::shared_ptr<Block> stmt)      override;
   std::any visit_IfStmt         (std::shared_ptr<If> stmt)         override;
   std::any visit_WhileStmt      (std::shared_ptr<While> stmt)      override;
   std::any visit_FunctionStmt   (std::shared_ptr<Function> stmt)   override;
   std::any visit_ReturnStmt     (std::shared_ptr<Return> stmt)     override;
   Interpreter();
   ~Interpreter() = default ;

   void interpret(std::vector<std::shared_ptr<Stmt>> staments);
   void execute_block(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> environment);

//* Environments can hold a reference to their enclosing (parent) environement and that is why we use a shared pointer 
public: std::shared_ptr<Environment> global_environment{std::make_shared<Environment>()};
private: std::shared_ptr<Environment> environment = global_environment;
   
private:
   std::any evaluate(std::shared_ptr<Expr> expr);
   void execute(std::shared_ptr<Stmt> stmt);
   bool is_truthy(std::any object);
   bool is_equal(std::any a, std::any b);
   void assert_number_operand(Token op, std::any object);
   void assert_number_operands(Token op, std::any left, std::any right);
   std::string stringify(std::any object);
};

class NativeClock: public LoxCallable {
public:
   int arity() override { return 0; }

   std::any call(Interpreter& interpreter, const std::vector<std::any> arguments) override 
   {
      auto ticks = std::chrono::system_clock::now().time_since_epoch();
      return std::chrono::duration<double>{ticks}.count() / 1000.0;
   }

   std::string to_string() override { return "<native fn>"; }
};