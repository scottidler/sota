#include "token.h"

namespace sota {

    /*Symbol*/

    Symbol::Symbol()
        : type(SymbolType::EndOfFile)
        , pattern("")
        , scan(nullptr)
        , nud(nullptr)
        , led(nullptr)
        , lbp(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp)
        : type(type)
        , pattern(pattern)
        , scan(scan)
        , nud(nud)
        , led(led)
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
        , length(0) {
    }

    Token::Token(const Symbol &symbol, const std::string &source, size_t index, size_t length)   
        : symbol(symbol)
        , source(source)
        , index(index)
        , length(length) {
    }

    std::string Token::value() const {
        return source.substr(index, length);
    }

    Ast * Token::nud(Parser *parser, Token *token) {
        return symbol.nud(parser, token);
    }

    Ast * Token::led(Parser *parser, Ast *ast, Token *token) {
        return symbol.led(parser, ast, token);
    }

    Token::operator bool() {
       return symbol == true;
    }

    std::ostream & operator<<(std::ostream &out, const Token &token) {
        return out << "Token(_symbol=" << token.symbol <<  ", Value=" << token.value() << ")";
    }
}
