#ifndef _LEXER_HPP
#define _LEXER_HPP

#include <queue>
#include <string>
#include <iostream>
#include "toy.hpp"

typedef enum {
    /* Expressions (with data attached to them) */
    tok_word, tok_number, tok_string,

    /* Binary Ops */
    tok_add, tok_sub, tok_mul,
    tok_div, tok_mod,
    tok_eq, tok_lt, tok_gt,
    tok_lte, tok_gte,

    /* Grammar */
    tok_paren_start, tok_paren_end,
    tok_block_start, tok_block_end,
    tok_semicolon, tok_comma,

    /* Statements */
    tok_while, tok_return, tok_def,
    tok_assign, tok_if, tok_else
} TokenType;

class Token {
  public:
    explicit Token(const TokenType type)
        : type_(type),
          number_(0),
          string_("") {}
    Token(const TokenType type, const std::string string)
        : type_(type),
          number_(0),
          string_(string) {}
    Token(const TokenType type, const double number)
        : type_(type),
          number_(number),
          string_("") {}

    static const std::string token_type_name(TokenType);
    const std::string name() const;

    inline TokenType type() const { return type_; }
    inline const std::string &string() const { return string_; }
    inline double number() const { return number_; }
  private:
    const TokenType type_;
    const double number_;
    const std::string string_;
    DISALLOW_COPY_AND_ASSIGN(Token);
};

class LexerContext {
  public:
    explicit LexerContext(std::istream &input)
        : input_(input),
          line_(1),
          filename_("<stdin>"),
          curtok_(0) {}
    ~LexerContext();

    bool fetchtok();

    inline const Token *curtok() const { return curtok_; }
    inline const std::string &filename() const { return filename_; }
    inline unsigned int line() const { return line_; }
    inline bool eos() const { return !input_.good(); }
  private:
    char next_char();
    bool next_char_equals(char);

    void strip_whitespace_and_comments();
    bool lex_string();
    bool lex_number();
    bool lex_word_or_keyword();
    bool lex_symbol();

    inline void set_curtok(Token *token) {
        if (curtok_)
            delete curtok_;
        curtok_ = token;
    }

    /* Data */
    std::queue<char> charbuf_;
    std::istream &input_;
    unsigned int line_;
    std::string filename_;
    Token *curtok_;
    DISALLOW_COPY_AND_ASSIGN(LexerContext);
};

#endif
