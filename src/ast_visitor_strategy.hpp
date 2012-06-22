#ifndef _AST_VISITOR_STRATEGY_HPP
#define _AST_VISITOR_STRATEGY_HPP

class ASTVisitor;

class ASTVisitorStrategy {
  public:
    virtual void dispatch(ASTVisitor*, const AST*) = 0;
    virtual void dispatch(ASTVisitor*, const ValueExpr*) = 0;
    virtual void dispatch(ASTVisitor*, const BinaryOpExpr*) = 0;
    virtual void dispatch(ASTVisitor*, const VariableExpr*) = 0;
    virtual void dispatch(ASTVisitor*, const AssignExpr*) = 0;
    virtual void dispatch(ASTVisitor*, const FuncCallExpr*) = 0;
    virtual void dispatch(ASTVisitor*, const ExpressionStatement*) = 0;
    virtual void dispatch(ASTVisitor*, const IfStatement*) = 0;
    virtual void dispatch(ASTVisitor*, const WhileStatement*) = 0;
    virtual void dispatch(ASTVisitor*, const ReturnStatement*) = 0;
    virtual void dispatch(ASTVisitor*, const DefStatement*) = 0;
};

#endif
