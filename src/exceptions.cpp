#include "exceptions.hpp"
#include <string>
#include <sstream>
#include "lexer.hpp"

UnexpectedCharacter::UnexpectedCharacter(char c) {
    std::ostringstream ss;
    ss << "Unexpected character: '" << c << "'";
    message_ = ss.str();
}

UnexpectedToken::UnexpectedToken(const std::string &where, const Token *token) {
    std::ostringstream ss;
    ss << "Unexpected token in " << where << "(): '" << token->name() << "'";
    message_ = ss.str();
}
