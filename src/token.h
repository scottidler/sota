#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <string>
#include <functional>

#include "ast.h"
#include "parser.h"
#include "exceptions.h"

namespace sota {

    class Ast;
    class Token;
    class Parser;

    enum BindPower: size_t {
        None = 0,
        Sum = 10,
        Product = 20,
    };

    //NAME          PATTERN     SCANNER             PARSER          LEFT_BIND_POWER
    #define SYMBOLS                                                                     \
    T(EndOfFile,    "",         EndOfFileScanner,   NullParser,     BindPower::None)    \
    T(WhiteSpace,   "[ \t]+",   RegexScanner,       NullParser,     BindPower::None)    \
    T(Add,          "+",        LiteralScanner,     InfixOperator , BindPower::Sum)     \
    T(Sub,          "-",        LiteralScanner,     InfixOperator , BindPower::Sum)     \
    T(Mul,          "*",        LiteralScanner,     InfixOperator , BindPower::Product) \
    T(Div,          "/",        LiteralScanner,     InfixOperator , BindPower::Product)  

    #define T(n,v,s,p,b) n,
    enum SymbolType {
        SYMBOLS
    };
    #undef T

    class Symbol {

        public:
        typedef std::function<size_t(const std::string &, size_t)> ScanFunc;
        typedef std::function<Ast*(Parser*, Ast*, Token)> ParseFunc;

        SymbolType      Type;
        std::string     Pattern;
        ScanFunc        Scan;
        ParseFunc       Parse;
        size_t          LBP;

        Symbol():
            Type(SymbolType::EndOfFile),
            Pattern(""),
            Scan(nullptr),
            Parse(nullptr),
            LBP(0) {
        }
        Symbol(SymbolType type, std::string pattern, ScanFunc scan, ParseFunc parse, size_t lbp):
            Type(type),
            Pattern(pattern),
            Scan(scan),
            Parse(parse),
            LBP(lbp) {
        }

        operator bool() {
            return Type != SymbolType::EndOfFile;
        }

        bool operator==(const Symbol &rhs) {
            return Type == rhs.Type;
        }

        bool operator!=(const Symbol &rhs) {
            return Type != rhs.Type;
        }

    };

    class Token {

        Symbol              *_symbol;

        public:
        const std::string   &Source;
        size_t              Index;
        size_t              Length;

        Token():
            _symbol(nullptr),
            Source(""),
            Index(0),
            Length(0) {
        }

        Token(Symbol *symbol, const std::string &source, size_t index, size_t length): 
            _symbol(symbol),
            Source(source),
            Index(index),
            Length(length) {
        }

        SymbolType Type() const {
            if (_symbol == nullptr)
                throw SotaException("_symbol == nullptr");
            return _symbol->Type;
        }

        std::string Pattern() const {
            return _symbol->Pattern;
        }

        size_t LBP() const {
            return _symbol->LBP;
        }

        size_t Scan(size_t index) const {
            return _symbol->Scan(Source, index);
        }

        std::string Value() const {
            return Source.substr(Index, Length);
        }

        Ast * Parse(Parser *parser, Ast *ast, Token token) const {
            return _symbol->Parse(parser, ast, token);
        }

        operator bool() {
           return *_symbol == true;
        }

        friend std::ostream & operator<<(std::ostream &out, const Token &token) {
            return out << token.Value();
        }
    };

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

    Ast * InfixOperator(Parser *parser, Ast *ast, Token token) {
        std::cout << "InfixOperator" << std::endl;
        return ast;
    }

    #define T(n,v,s,p,b) std::make_pair(SymbolType::n, Symbol(SymbolType::n, v, s, p, b) ),
    static std::map<SymbolType, Symbol> Type2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(n,v,s,p,b) std::make_pair(v, Symbol(SymbolType::n, v, s, p, b) ),
    static std::map<std::string, Symbol> Name2Symbol {
        SYMBOLS
    };
    #undef T
}

#endif /*__SOTA_TOKEN__*/
