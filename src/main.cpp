#include <iostream>
#include "exceptions.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "toy.hpp"
#include "pprinter_visitor.hpp"
#include "ast_depth_first.hpp"

int main() {
    LexerContext lexer(std::cin);
    ParserContext parse(lexer);

    AST *ast = 0;

    try {
        ast = parse.parse_ast(false);
    } catch (SyntaxError &error) {
        std::cout << error.message() << std::endl;
        return 1;
    }

    PrettyPrinterVisitor ppv;
    ASTVisitorDepthFirst df_strategy;
    ast->accept(&df_strategy, &ppv);

    std::cout << ppv.buffer();

    delete ast;
    return 0;
}
