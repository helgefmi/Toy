#include "lexer.hpp"
#include <cstdlib>
#include <iostream>
#include <queue>
#include <sstream>
#include <cassert>
#include "exceptions.hpp"

#define toy_isnumeric(c) (isdigit((c)) || (c) == '.')
#define toy_isalphanumeric(c) (isalpha((c)) || isdigit((c)) || (c) == '_')

#define MAX_SYNTAX_CHARS ((int)sizeof("return"))

const std::string Token::token_type_name(TokenType type) {
    static std::string tok_name_table[] = {
        "word", "number", "string",

        "add",  "sub", "mul",
        "div",  "mod",
        "eq",   "lt", "gt",
        "lte",  "gte",

        "paren_start", "paren_end",
        "block_start", "block_end",
        "semicolon", "comma",

        "while",  "return", "def",
        "assign", "if",     "else"
    };

    return tok_name_table[(int)type];
}

const std::string Token::name() const {
    std::ostringstream ss;
    ss << Token::token_type_name(type_);

    if (type_ == tok_word || type_ == tok_string) {
        ss << ":" << string_;
    } else if (type_ == tok_number) {
            ss << ":" << number_;
    }

    return ss.str();
}

LexerContext::~LexerContext() {
    if (curtok_)
        delete curtok_;
}

/* Getting input */

char LexerContext::next_char() {
    if (charbuf_.size() > 0) {
        char c = charbuf_.front();
        charbuf_.pop();

        return c;
    }

    return input_.get();
}

bool LexerContext::next_char_equals(char eq) {
    char c  = next_char();
    if (c == eq)
        return true;

    charbuf_.push(c);
    return false;
}

/* Parsing input */

void LexerContext::strip_whitespace_and_comments() {
    char c = next_char();

    while (isspace(c) || c == '#') {
        if (c == '\n')
            ++line_;

        if (c == '#') {
            do {
                c = next_char();
            } while (!eos() && c != '\n');
        } else {
            c = next_char();
        }
    }

    charbuf_.push(c);
}

bool LexerContext::lex_string() {
    char c = next_char();

    if (c == '"') {
        std::string string; /* TODO: substr algorithm? */

        while ('"' != (c = next_char())) {
            if (eos()) {
                throw SyntaxError("Unterminated string; expecting '\"'");
                return false;
            } else if (c == '\n') {
                ++line_;
            }

            string.push_back(c);
        }

        set_curtok(new Token(tok_string, string));
        return true;
    }

    charbuf_.push(c);
    return false;
}

bool LexerContext::lex_number() {
    char c = next_char();

    if (toy_isnumeric(c)) {
        std::string number;

        while (toy_isnumeric(c)) {
            number.push_back(c);
            c = next_char();
        }

        charbuf_.push(c);

        double number_value = strtod(number.c_str(), 0);
        set_curtok(new Token(tok_number, number_value));

        return true;
    }

    charbuf_.push(c);
    return false;
}

bool LexerContext::lex_word_or_keyword() {
    char c = next_char();

    if (isalpha(c)) {
        std::string word;

        while (toy_isalphanumeric(c)) {
            word.push_back(c);
            c = next_char();
        }

        charbuf_.push(c);

        if (word == "def") set_curtok(new Token(tok_def));
        else if (word == "if") set_curtok(new Token(tok_if));
        else if (word == "else") set_curtok(new Token(tok_else));
        else if (word == "while") set_curtok(new Token(tok_while));
        else if (word == "return") set_curtok(new Token(tok_return));
        else set_curtok(new Token(tok_word, word));

        return true;
    }

    charbuf_.push(c);
    return false;
}

bool LexerContext::lex_symbol() {
    char c = next_char();

    switch (c) {
        case '=': {
            if (next_char_equals('=')) set_curtok(new Token(tok_eq));
            else set_curtok(new Token(tok_assign));
            break;
        }
        case '<': {
            if (next_char_equals('=')) set_curtok(new Token(tok_lte));
            else set_curtok(new Token(tok_lt));
            break;
        }
        case '>': {
            if (next_char_equals('=')) set_curtok(new Token(tok_gte));
            else set_curtok(new Token(tok_gt));
            break;
        }
        case '(': set_curtok(new Token(tok_paren_start)); break;
        case ')': set_curtok(new Token(tok_paren_end)); break;
        case '{': set_curtok(new Token(tok_block_start)); break;
        case '}': set_curtok(new Token(tok_block_end)); break;
        case '+': set_curtok(new Token(tok_add)); break;
        case '-': set_curtok(new Token(tok_sub)); break;
        case '*': set_curtok(new Token(tok_mul)); break;
        case '/': set_curtok(new Token(tok_div)); break;
        case '%': set_curtok(new Token(tok_mod)); break;
        case ';': set_curtok(new Token(tok_semicolon)); break;
        case ',': set_curtok(new Token(tok_comma)); break;

        default: {
            charbuf_.push(c);
            return false;
        }
    }

    return true;
}

bool LexerContext::fetchtok() {
    strip_whitespace_and_comments();

    if (eos())
        return false;

    if (lex_string())
        return true;

    if (lex_number())
        return true;

    if (lex_word_or_keyword())
        return true;

    if (lex_symbol())
        return true;

    throw UnexpectedCharacter(next_char());
}
