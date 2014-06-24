#include "token.h"

namespace sota {

    /*Symbol*/

    Symbol::Symbol()
        : Type(SymbolType::EndOfFile)
        , Pattern("")
        , Scan(nullptr)
        , Nud(nullptr)
        , Led(nullptr)
        , LBP(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp)
        : Type(type)
        , Pattern(pattern)
        , Scan(scan)
        , Nud(nud)
        , Led(led)
        , LBP(lbp) {
    }

    Symbol::operator bool() {
        return Type != SymbolType::EndOfFile;
    }

    bool Symbol::operator==(const Symbol &rhs) {
        return Type == rhs.Type;
    }

    bool Symbol::operator!=(const Symbol &rhs) {
        return Type != rhs.Type;
    }

    std::ostream & operator<<(std::ostream &out, const Symbol &symbol) {
        return out << symbol.Type;
    }

    /*Token*/

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
