#include "ast.hpp"

void AST::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    for (std::vector<const Statement*>::const_iterator it = nodes_.begin(), end = nodes_.end(); it != end; ++it) {
        (*it)->accept(s, v);
    }
}

void ValueExpr::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
}
void BinaryOpExpr::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    left_->accept(s, v);
    right_->accept(s, v);
}
void VariableExpr::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
}
void AssignExpr::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    rvalue_->accept(s, v);
}
void FuncCallExpr::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    for (std::vector<const Expression*>::const_iterator it = args_.begin(), end = args_.end(); it != end; ++it) {
        (*it)->accept(s, v);
    }
}
void ExpressionStatement::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    expr_->accept(s, v);
}
void IfStatement::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    cond_->accept(s, v);
    true_block_->accept(s, v);
    if (false_block_)
        false_block_->accept(s, v);
}
void WhileStatement::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    cond_->accept(s, v);
    block_->accept(s, v);
}
void ReturnStatement::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    ret_->accept(s, v);
}
void DefStatement::accept(ASTVisitorStrategy *s, ASTVisitor *v) const {
    s->dispatch(v, this);
    block_->accept(s, v);
}
