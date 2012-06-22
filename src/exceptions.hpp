#ifndef _EXCEPTIONS_HPP
#define _EXCEPTIONS_HPP

#include <string>

class Token;

class SyntaxError {
    public:
        SyntaxError() {}
        explicit SyntaxError(const std::string &message) : message_(message) {}
        inline const std::string &message() const { return message_; }
    protected:
        std::string message_;
};

class UnexpectedCharacter : public SyntaxError {
    public:
        UnexpectedCharacter(char);
};

class UnexpectedToken : public SyntaxError {
    public:
        UnexpectedToken(const std::string&, const Token*);
};

#endif
