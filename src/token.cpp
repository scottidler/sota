#include "token.h"

namespace sota {

    /*Symbol*/

    Symbol::Symbol()
        : type(SymbolType::EndOfFile)
        , pattern("")
        , Scan(nullptr)
        , Nud(nullptr)
        , Led(nullptr)
        , lbp(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp)
        : type(type)
        , pattern(pattern)
        , Scan(scan)
        , Nud(nud)
        , Led(led)
        , lbp(lbp) {
    }

    Symbol::operator bool() {
        return type != SymbolType::EndOfFile;
    }

    bool Symbol::operator==(const Symbol &rhs) {
        return type == rhs.type;
    }

    bool Symbol::operator!=(const Symbol &rhs) {
        return type != rhs.type;
    }

    std::ostream & operator<<(std::ostream &out, const Symbol &symbol) {
        return out << "Symbol(type=" << symbol.type << ", pattern=" << symbol.pattern << ", lbp=" << symbol.lbp <<  ")";
    }

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
