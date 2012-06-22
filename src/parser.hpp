#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "toyobj.hpp"
#include "lexer.hpp"
#include "toy.hpp"
#include "ast.hpp"

class ParserContext {
  public:
    explicit ParserContext(LexerContext &lexer)
        : lexer_(lexer) { lexer_.fetchtok(); }

    AST *parse_ast(bool);
 private:
    Statement *parse_statement();
    Statement *parse_while();
    Statement *parse_if();
    Statement *parse_return();
    Statement *parse_def();

    Expression *parse_expression();
    Expression *parse_primary();
    Expression *parse_binary_op_expression(Expression*, int);
    Expression *parse_paren_expression();
    Expression *parse_number();
    Expression *parse_string();
    Expression *parse_word_expression();

    AST *parse_block();

    int get_prec(TokenType) const;
    inline const Token *curtok() { return lexer_.curtok(); }
    inline void eat_token(TokenType type) {
        if (type != curtok()->type()) {
            std::cout << "I was expecting " << Token::token_type_name(type) << " but got " << curtok()->name() << std::endl;
            exit(1);
        }
        lexer_.fetchtok();
    }

    LexerContext &lexer_;
    DISALLOW_COPY_AND_ASSIGN(ParserContext);
};

#endif
