#include "token.h"
#include "symbol.h"

namespace sota {

    Token::Token()
        : _symbol(nullptr)
        , _source("")
        , Index(0)
        , Length(0) {
    }

    Token::Token(Symbol *symbol, const std::string &source, size_t index, size_t length)   
        : _symbol(symbol)
        , _source(source)
        , Index(index)
        , Length(length) {
    }

    SymbolType Token::Type() const {
        if (_symbol == nullptr)
            throw SotaException("_symbol == nullptr");
        return _symbol->Type;
    }

    std::string Token::Pattern() const {
        return _symbol->Pattern;
    }

    size_t Token::LBP() const {
        return _symbol->LBP;
    }

    std::string Token::Value() const {
        return _source.substr(Index, Length);
    }

    Ast * Token::Nud(Parser *parser, Token *token) {
        return _symbol->Nud(parser, token);
    }

    Ast * Token::Led(Parser *parser, Ast *ast, Token *token) {
        return _symbol->Led(parser, ast, token);
    }

    Token::operator bool() {
       return *_symbol == true;
    }

    std::ostream & operator<<(std::ostream &out, const Token &token) {
        return out << token.Value();
    }
}
