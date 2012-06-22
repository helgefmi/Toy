#include "parser.hpp"
#include <vector>
#include "exceptions.hpp"
#include "lexer.hpp"

int ParserContext::get_prec(TokenType op) const {
    switch (op) {
        case tok_add:
        case tok_sub:
            return 0;
        case tok_div:
        case tok_mod:
            return 1;
        case tok_mul:
            return 2;
        case tok_lt:
        case tok_gt:
        case tok_lte:
        case tok_gte:
            return 3;
        case tok_eq:
            return 4;
        default: return -1;
    }
}

AST *ParserContext::parse_ast(bool in_block) {
    std::vector<const Statement*> statements;

    while (!lexer_.eos() && !(in_block && curtok()->type() == tok_block_end)) {
        statements.push_back(parse_statement());
    }

    return new AST(statements);
}

Statement *ParserContext::parse_statement() {
    Statement *statement = 0;
    switch (curtok()->type()) {
        case tok_while: {
            statement = parse_while();
            break;
        }
        case tok_if: {
            statement = parse_if();
            break;
        }
        case tok_return: {
            statement = parse_return();
            eat_token(tok_semicolon);
            break;
        }
        case tok_def: {
            statement = parse_def();
            break;
        }
        default: {
            Expression *expression = parse_expression();
            if (expression) {
                statement = new ExpressionStatement(expression);
                eat_token(tok_semicolon);
            } else {
                throw UnexpectedToken("parse_statement", curtok());
            }
            break;
        }
    }
    return statement;
}

Expression *ParserContext::parse_expression() {
    Expression *ret = parse_primary();

    if (ret->type() == toy_assign)
        return ret;

    return parse_binary_op_expression(ret, 0);
}

AST *ParserContext::parse_block() {
    AST *ret = 0;

    if (curtok()->type() == tok_block_start) {
        eat_token(tok_block_start);
        if (curtok()->type() == tok_block_end) {
            ret = new AST(std::vector<const Statement*>());
        } else {
            ret = parse_ast(true);
        }
        eat_token(tok_block_end);
    } else {
        std::vector<const Statement*> statements;
        statements.push_back(parse_statement());
        ret = new AST(statements);
    }

    return ret;
}

/* Statatements */
Statement *ParserContext::parse_while() {
    eat_token(tok_while);
    Expression *cond = parse_paren_expression();
    AST *block = parse_block();

    return new WhileStatement(cond, block);
}

Statement *ParserContext::parse_if() {
    eat_token(tok_if);
    Expression *cond = parse_paren_expression();
    AST *true_block = parse_block();

    if (curtok()->type() == tok_else) {
        AST *false_block = parse_block();
        return new IfStatement(cond, true_block, false_block);
    }

    return new IfStatement(cond, true_block);
}

Statement *ParserContext::parse_return() {
    eat_token(tok_return);
    return new ReturnStatement(parse_expression());
}

Statement *ParserContext::parse_def() {
    eat_token(tok_def);

    if (curtok()->type() != tok_word)
        throw SyntaxError("Expected identifier in function definition");

    std::string funcname = curtok()->string();
    eat_token(tok_word);

    if (curtok()->type() != tok_paren_start)
        throw SyntaxError("Expected parenthesis in function definition");
    eat_token(tok_paren_start);

    std::vector<std::string> params;
    while (curtok()->type() != tok_paren_end) {
        if (curtok()->type() != tok_word)
            throw SyntaxError("Parameters must be identifiers in function definitions");

        params.push_back(curtok()->string());
        eat_token(tok_word);

        if (curtok()->type() == tok_comma)
            eat_token(tok_comma);
    }
    eat_token(tok_paren_end);

    return new DefStatement(funcname, params, parse_block());
}

/* Expressions */

Expression *ParserContext::parse_primary() {
    switch (curtok()->type()) {
        case tok_word: return parse_word_expression();
        case tok_number: return parse_number();
        case tok_string: return parse_string();
        case tok_paren_start: return parse_paren_expression();
        default: throw UnexpectedToken("parse_primary", curtok());
    }
}

Expression *ParserContext::parse_binary_op_expression(Expression *LHS, int min_prec) {
    int op_prec = get_prec(curtok()->type());

    while (op_prec >= min_prec) {
        TokenType op = curtok()->type();
        eat_token(op);

        Expression *RHS = parse_primary();

        int next_prec = get_prec(curtok()->type());
        while (next_prec > op_prec) {
            RHS = parse_binary_op_expression(RHS, next_prec);
            next_prec = get_prec(curtok()->type());
        }

        LHS = new BinaryOpExpr(LHS, RHS, op);

        op_prec = get_prec(curtok()->type());
    }

    return LHS;
}

Expression *ParserContext::parse_paren_expression() {
    eat_token(tok_paren_start);
    Expression *ret = parse_expression();
    eat_token(tok_paren_end);

    return ret;
}

Expression *ParserContext::parse_number() {
    Expression *ret = new ValueExpr(curtok()->number());
    eat_token(tok_number);
    return ret;
}

Expression *ParserContext::parse_string() {
    Expression *ret = new ValueExpr(curtok()->string());
    eat_token(tok_string);
    return ret;
}

Expression *ParserContext::parse_word_expression() {
    std::string word = curtok()->string();
    eat_token(tok_word);

    if (curtok()->type() == tok_assign)
    {
        eat_token(tok_assign);
        return new AssignExpr(word, parse_expression());
    } else if (curtok()->type() == tok_paren_start) {
        eat_token(tok_paren_start);

        std::vector<const Expression*> args;
        while (curtok()->type() != tok_paren_end) {
            args.push_back(parse_expression());

            if (curtok()->type() == tok_comma)
                eat_token(tok_comma);
        }
        eat_token(tok_paren_end);

        return new FuncCallExpr(word, args);
    }

    return new VariableExpr(word);
}
