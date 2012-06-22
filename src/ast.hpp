#ifndef _AST_HPP
#define _AST_HPP

#include <vector>
#include <string>
#include "toy.hpp"
#include "lexer.hpp"
#include "ast_visitor.hpp"
#include "ast_visitor_strategy.hpp"

typedef enum {
    toy_number,
    toy_string,
    toy_binary_op,
    toy_variable,

    toy_expression_statement,
    toy_if,
    toy_return,
    toy_while,
    toy_def,
    toy_assign,
    toy_function_call,

    toy_ast
} NodeType;

/* Building blocks */
class ASTNode {
  public:
    ASTNode() {};
    NodeType type() const { return type_; }
    virtual void accept(ASTVisitorStrategy*, ASTVisitor*) const = 0;
  protected:
    explicit ASTNode(NodeType type) : type_(type) {}
    NodeType type_;
  private:
    DISALLOW_COPY_AND_ASSIGN(ASTNode);
};

class Statement : public virtual ASTNode {
  public:
    Statement() {}
  private:
    DISALLOW_COPY_AND_ASSIGN(Statement);
};

class Expression : public virtual ASTNode {
  public:
    Expression() {}
  private:
    DISALLOW_COPY_AND_ASSIGN(Expression);
};

class AST : public ASTNode {
  public:
    AST(const std::vector<const Statement*> &nodes)
        : ASTNode(toy_ast),
          nodes_(nodes) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const std::vector<const Statement*> nodes() const { return nodes_; }
  private:
    const std::vector<const Statement*> nodes_;
    DISALLOW_COPY_AND_ASSIGN(AST);
};

/* Expressions */
class ValueExpr : public Expression {
  public:
    ValueExpr(const std::string &string)
        : ASTNode(toy_string),
          string_(string),
          number_(0) {}
    ValueExpr(double number)
        : ASTNode(toy_number),
          string_(""),
          number_(number) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;

    inline const std::string &string() const { return string_; }
    inline double number() const { return number_; }
    inline bool is_string() const { return type_ == toy_string; }
    inline bool is_number() const { return type_ == toy_number; }
  private:
    const std::string string_;
    const double number_;
    DISALLOW_COPY_AND_ASSIGN(ValueExpr);
};

class BinaryOpExpr : public Expression {
  public:
    BinaryOpExpr(const Expression *left, const Expression *right, const TokenType op_type)
        : ASTNode(toy_binary_op),
          left_(left),
          right_(right),
          op_type_(op_type) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline TokenType op_type() const { return op_type_; }
    inline const Expression *left() const { return left_; }
    inline const Expression *right() const { return right_; }
  private:
    const Expression *left_;
    const Expression *right_;
    const TokenType op_type_;
    DISALLOW_COPY_AND_ASSIGN(BinaryOpExpr);
};

class VariableExpr : public Expression {
  public:
    VariableExpr(const std::string &varname)
        : ASTNode(toy_variable),
          varname_(varname) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const std::string varname() const { return varname_; }
  private:
    const std::string varname_;
    DISALLOW_COPY_AND_ASSIGN(VariableExpr);
};

class AssignExpr : public Expression {
  public:
    AssignExpr(const std::string &lvalue, const Expression *rvalue)
        : ASTNode(toy_assign),
          lvalue_(lvalue),
          rvalue_(rvalue) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const std::string lvalue() const { return lvalue_; }
    inline const Expression *rvalue() const { return rvalue_; }
  private:
    const std::string lvalue_;
    const Expression *rvalue_;
    DISALLOW_COPY_AND_ASSIGN(AssignExpr);
};

class FuncCallExpr : public Expression {
  public:
    FuncCallExpr(const std::string &funcname, const std::vector<const Expression*> &args)
        : ASTNode(toy_function_call),
          funcname_(funcname),
          args_(args) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const std::string funcname() const { return funcname_; }
    inline const std::vector<const Expression*> args() const { return args_; }
  private:
    const std::string funcname_;
    const std::vector<const Expression*> args_;
    DISALLOW_COPY_AND_ASSIGN(FuncCallExpr);
};

/* Statements */
class ExpressionStatement : public Statement {
  public:
    explicit ExpressionStatement(const Expression *expr)
        : ASTNode(toy_expression_statement),
          expr_(expr) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const Expression *expr() const { return expr_; }
  private:
    const Expression *expr_;
    DISALLOW_COPY_AND_ASSIGN(ExpressionStatement);
};

class IfStatement : public Statement {
  public:
    IfStatement(const Expression *cond, const AST *true_block)
        : ASTNode(toy_if),
          cond_(cond),
          true_block_(true_block),
          false_block_(0) {}
    IfStatement(const Expression *cond, const AST *true_block, const AST *false_block)
        : ASTNode(toy_if),
          cond_(cond),
          true_block_(true_block),
          false_block_(false_block) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const Expression *cond() const { return cond_; }
    inline const AST *true_block() const { return true_block_; }
    inline const AST *false_block() const { return false_block_; }
  private:
    const Expression *cond_;
    const AST *true_block_, *false_block_;
    DISALLOW_COPY_AND_ASSIGN(IfStatement);
};

class WhileStatement : public Statement {
  public:
    WhileStatement(const Expression *cond, const AST *block)
        : ASTNode(toy_while),
          cond_(cond),
          block_(block) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const Expression *cond() const { return cond_; }
    inline const AST *block() const { return block_; }
  private:
    const Expression *cond_;
    const AST *block_;
    DISALLOW_COPY_AND_ASSIGN(WhileStatement);
};

class ReturnStatement : public Statement {
  public:
    ReturnStatement(const Expression *ret)
        : ASTNode(toy_return),
          ret_(ret) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
  inline const Expression *ret() const { return ret_; }
  private:
    const Expression *ret_;
    DISALLOW_COPY_AND_ASSIGN(ReturnStatement);
};

class DefStatement : public Statement {
  public:
    DefStatement(const std::string &name, const std::vector<std::string> &params, const AST *block)
        : ASTNode(toy_def),
          name_(name),
          params_(params),
          block_(block) {}
    void accept(ASTVisitorStrategy*, ASTVisitor *v) const;
    inline const std::string name() const { return name_; }
    inline const std::vector<std::string> params() const { return params_; }
    inline const AST *block() const { return block_; }
  private:
    const std::string name_;
    const std::vector<std::string> params_;
    const AST *block_;
    DISALLOW_COPY_AND_ASSIGN(DefStatement);
};

#endif
