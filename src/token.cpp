#include "token.h"
#include "symbol.h"

namespace sota {

    /*
    Symbol::Symbol():
        Type(SymbolType::EndOfFile),
        Pattern(""),
        Scan(nullptr),
        Parse(nullptr),
        LBP(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, ParseFunc parse, size_t lbp):
        Type(type),
        Pattern(pattern),
        Scan(scan),
        Parse(parse),
        LBP(lbp) {
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
    */




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

    size_t Token::Scan(size_t index) const {
        return _symbol->Scan(Source, index);
    }

    std::string Token::Value() const {
        return Source.substr(Index, Length);
    }

    Ast * Token::Parse(Parser *parser, Ast *ast, Token token) const {
        return _symbol->Parse(parser, ast, token);
    }

    Token::operator bool() {
       return *_symbol == true;
    }

    std::ostream & operator<<(std::ostream &out, const Token &token) {
        return out << token.Value();
    }











    size_t EndOfFileScanner(const std::string &source, size_t index) {
        std::cout << "EndOfFileScanner" << std::endl;
        return index;
    }

    size_t RegexScanner(const std::string &source, size_t index) {
        std::cout << "RegexScanner" << std::endl;
        return index;
    }

    size_t LiteralScanner(const std::string &source, size_t index) {
        std::cout << "LiteralScanner" << std::endl;
        return index;
    }

    Ast * NullParser(Parser *parser, Ast *ast, Token token) {
        std::cout << "NullParser" << std::endl;
        return ast;
    }

    Ast * InfixParser(Parser *parser, Ast *ast, Token token) {
        std::cout << "InfixOperator" << std::endl;
        return ast;
    }

    /*
    #define T(n,v,s,p,b) std::make_pair(SymbolType::n, new Symbol(SymbolType::n, v, s, p, b) ),
    static std::map<SymbolType, Symbol *> Type2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(n,v,s,p,b) std::make_pair(v, new Symbol(SymbolType::n, v, s, p, b) ),
    static std::map<std::string, Symbol *> Name2Symbol {
        SYMBOLS
    };
    #undef T
    */
}
