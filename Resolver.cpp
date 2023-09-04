#include "headers/Resolver.h"
#include "headers/Lox.h"

Resolver::Resolver(Interpreter interpreter)
  : interpreter(interpreter)
{}


std::any Resolver::visit_BlockStmt(std::shared_ptr<Block> stmt)
{  
   begin_scope();
   resolve(stmt->statements);
   end_scope();

   return nullptr;
}

std::any Resolver::visit_FunctionStmt(std::shared_ptr<Function> stmt)
{
   declare(stmt->name);
   define(stmt->name);

   resolve_function(stmt);
   return nullptr;
}

std::any Resolver::visit_VarStmt(std::shared_ptr<Var> stmt)
{
   declare(stmt->name);
   if (stmt->initializer != nullptr)
   {
      resolve(stmt->initializer);
   }
   define(stmt->name);

   return nullptr;
}

std::any Resolver::visit_ExpressionStmt(std::shared_ptr<Expression> stmt)
{
   resolve(stmt->expression);
   return nullptr;
}

std::any Resolver::visit_IfStmt(std::shared_ptr<If> stmt)
{
   resolve(stmt->condition);
   resolve(stmt->then_branch);
   if (stmt->else_branch != nullptr) { resolve(stmt->else_branch); }
   return nullptr;
}

std::any Resolver::visit_PrintStmt(std::shared_ptr<Print> stmt)
{
   resolve(stmt->expression);
   return nullptr;
}

std::any Resolver::visit_ReturnStmt(std::shared_ptr<Return> stmt)
{
   if (stmt->value != nullptr) {
      resolve(stmt->value);
   }

   return nullptr;
}

std::any Resolver::visit_WhileStmt(std::shared_ptr<While> stmt)
{
   resolve(stmt->condition);
   resolve(stmt->body);
   return nullptr;
}


std::any Resolver::visit_AssignExpr(std::shared_ptr<Assign> expr)
{
   resolve(expr->value);
   resolve_local(expr, expr->name);
   return nullptr;
}

std::any Resolver::visit_VariableExpr(std::shared_ptr<Variable> expr)
{
   if (!scopes.empty())
   {
      auto& scope = scopes.back();
      auto elem = scope.find(expr->name.lexeme);
      if (elem != scope.end() && elem->second == false){
         Lox::error(expr->name, "Can't read local variable in its own initializer.");
      }
   }
   resolve_local(expr, expr->name);
   return nullptr;
}

std::any Resolver::visit_BinaryExpr(std::shared_ptr<Binary> expr)
{
   resolve(expr->left);
   resolve(expr->right);
   return nullptr;
}

std::any Resolver::visit_CallExpr(std::shared_ptr<Call> expr)
{
   resolve(expr->calle);

   for (std::shared_ptr<Expr> argument : expr->arguements) {
      resolve(argument);
   }

   return nullptr;
}

std::any Resolver::visit_GroupExpr(std::shared_ptr<Group> expr)
{
   resolve(expr->expr_in);
   return nullptr;
}

std::any Resolver::visit_LiteralExpr(std::shared_ptr<Literal> expr)
{
   return nullptr;
}

std::any Resolver::visit_LogicalExpr(std::shared_ptr<Logical> expr)
{
   resolve(expr->left);
   resolve(expr->right);
   return nullptr;
}

std::any Resolver::visit_UnaryExpr(std::shared_ptr<Unary> expr)
{
   resolve(expr->right);
   return nullptr;
}

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements)
{
   for (std::shared_ptr<Stmt> stmt : statements) {
      resolve(stmt);
   }
}

void Resolver::resolve(std::shared_ptr<Stmt> stmt)
{
   stmt->accept(*this);
}

void Resolver::resolve(std::shared_ptr<Expr> expr)
{
   expr->accept(*this);
}

void Resolver::begin_scope()
{
   scopes.push_back(std::map<std::string, bool>{});
}

void Resolver::end_scope()
{
   scopes.pop_back();
}

void Resolver::declare(Token name)
{
   if (scopes.empty()) { return; }
   scopes.back()[name.lexeme] = false;
}

void Resolver::define(Token name)
{
   if (scopes.empty()) { return; }
   scopes.back()[name.lexeme] = true;
}

void Resolver::resolve_local(std::shared_ptr<Expr> expr, Token name)
{
   for (int i = scopes.size()-1 ; i>= 0; i--)
   {
      if (scopes[i].find(name.lexeme) != scopes[i].end() )
      {
         interpreter.resolve(expr, scopes.size() - 1 - i);
         return;
      }
   }
}

void Resolver::resolve_function(std::shared_ptr<Function> function)
{
   begin_scope();
   for (Token param : function->params) {
      declare(param);
      define(param);
   }
   resolve(function->body);
   end_scope();
}