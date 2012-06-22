#ifndef _AST_VISITOR_HPP
#define _AST_VISITOR_HPP

class AST;
class ValueExpr;
class BinaryOpExpr;
class VariableExpr;
class AssignExpr;
class FuncCallExpr;
class ExpressionStatement;
class IfStatement;
class WhileStatement;
class ReturnStatement;
class DefStatement;

class ASTVisitor {
  public:
    virtual void visit(const AST*) = 0;
    virtual void visit(const ValueExpr*) = 0;
    virtual void visit(const BinaryOpExpr*) = 0;
    virtual void visit(const VariableExpr*) = 0;
    virtual void visit(const AssignExpr*) = 0;
    virtual void visit(const FuncCallExpr*) = 0;
    virtual void visit(const ExpressionStatement*) = 0;
    virtual void visit(const IfStatement*) = 0;
    virtual void visit(const WhileStatement*) = 0;
    virtual void visit(const ReturnStatement*) = 0;
    virtual void visit(const DefStatement*) = 0;
};

#endif
