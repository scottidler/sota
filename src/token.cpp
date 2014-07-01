#include "token.h"

namespace sota {

    /*Token*/

    Token::Token()
        : symbol(Symbol())
        , source("")
        , index(0)
        , length(0)
        , skip(false) {
    }

    Token::Token(const Symbol &symbol, const std::string &source, size_t index, size_t length, bool skip)
        : symbol(symbol)
        , source(source)
        , index(index)
        , length(length)
        , skip(skip) {
    }

    std::string Token::Value() const {
        return source.substr(index, length);
    }

    Ast * Token::Nud(Parser *parser, Token *token) {
        return symbol.Nud(parser, token);
    }

    Ast * Token::Led(Parser *parser, Ast *ast, Token *token) {
        return symbol.Led(parser, ast, token);
    }

    Token::operator bool() {
       return symbol == true;
    }

    std::ostream & operator<<(std::ostream &out, const Token &token) {
        return out << "Token(symbol=" << token.symbol <<  ", Value()=" << token.Value() << ")";
    }
}
