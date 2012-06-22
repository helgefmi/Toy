#ifndef _AST_DEPTH_FIRST_HPP
#define _AST_DEPTH_FIRST_HPP

#include "ast.hpp"
#include "ast_visitor.hpp"
#include "ast_visitor_strategy.hpp"
#include <vector>

class ASTVisitorDepthFirst : public ASTVisitorStrategy {
  public:
    inline void dispatch(ASTVisitor *v, const AST *node) {
        v->visit(node);
        for (std::vector<const Statement*>::const_iterator it = node->nodes().begin(), end = node->nodes().end(); it != end; ++it) {
            //v->visit(*it);
        }
    }

    inline void dispatch(ASTVisitor *v, const ValueExpr *node) {
        v->visit(node);
    }

    inline void dispatch(ASTVisitor *v, const BinaryOpExpr *node) {
        v->visit(node);
        //v->visit(node->left());
        //v->visit(node->right());
    }

    inline void dispatch(ASTVisitor *v, const VariableExpr *node) {
        v->visit(node);
    }

    inline void dispatch(ASTVisitor *v, const AssignExpr *node) {
        v->visit(node);
        //v->visit(node->rvalue());
    }

    inline void dispatch(ASTVisitor *v, const FuncCallExpr *node) {
        v->visit(node);
        for (std::vector<const Expression*>::const_iterator it = node->args().begin(), end = node->args().end(); it != end; ++it) {
            //v->visit(*it);
        }
    }

    inline void dispatch(ASTVisitor *v, const ExpressionStatement *node) {
        v->visit(node);
        //v->visit(node->expr());
    }

    inline void dispatch(ASTVisitor *v, const IfStatement *node) {
        v->visit(node);
        //v->visit(node->cond());
        v->visit(node->true_block());
        if (node->false_block())
            v->visit(node->false_block());
    }

    inline void dispatch(ASTVisitor *v, const WhileStatement *node) {
        v->visit(node);
        //v->visit(node->cond());
        //v->visit(node->block());
    }

    inline void dispatch(ASTVisitor *v, const ReturnStatement *node) {
        v->visit(node);
        //v->visit(node->ret());
    }

    inline void dispatch(ASTVisitor *v, const DefStatement *node) {
        v->visit(node);
        v->visit(node->block());
    }
};

#endif
