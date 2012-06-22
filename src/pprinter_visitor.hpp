#ifndef _AST_PPRINTER_VISITOR
#define _AST_PPRINTER_VISITOR

#include <string>
#include "ast_visitor.hpp"
#include "ast.hpp"

class PrettyPrinterVisitor : public ASTVisitor {
  public:
    PrettyPrinterVisitor()
        : ident_(0),
          buffer_("") {}

    virtual void visit(const AST*);
    virtual void visit(const ValueExpr*);
    virtual void visit(const BinaryOpExpr*);
    virtual void visit(const VariableExpr*);
    virtual void visit(const AssignExpr*);
    virtual void visit(const FuncCallExpr*);
    virtual void visit(const ExpressionStatement*);
    virtual void visit(const IfStatement*);
    virtual void visit(const WhileStatement*);
    virtual void visit(const ReturnStatement*);
    virtual void visit(const DefStatement*);

    inline std::string buffer() { return buffer_; }
  private:
    void append_ident();
    int ident_;
    std::string buffer_;
    DISALLOW_COPY_AND_ASSIGN(PrettyPrinterVisitor);
};

#endif
