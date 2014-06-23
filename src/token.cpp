#include "token.h"
#include "symbol.h"

namespace sota {

    Token::Token()
        : _symbol(nullptr)
        , Source("")
        , Index(0)
        , Length(0) {
    }

    Token::Token(Symbol *symbol, const std::string &source, size_t index, size_t length)   
        : _symbol(symbol)
        , Source(source)
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
        return Source.substr(Index, Length);
    }

    size_t Token::Scan(size_t index) {
        return _symbol->Scan(Source, index);
    }

    Ast * Token::Parse(Parser *parser, Ast *ast, Token *token) {
        return _symbol->Parse(parser, ast, token);
    }

    Token::operator bool() {
       return *_symbol == true;
    }

    std::ostream & operator<<(std::ostream &out, const Token &token) {
        return out << token.Value();
    }
}
