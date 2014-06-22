#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <string>
#include <functional>

//#include "ast.h"
//#include "parser.h"
#include "exceptions.h"

#include "grammar.h"

namespace sota {

    class Ast;
    class Token;
    class Parser;
    class Symbol;

    enum BindPower: size_t {
        None = 0,
        Sum = 10,
        Product = 20,
    };

    /*
    //NAME          PATTERN     SCANNER             PARSER          LEFT_BIND_POWER
    #define SYMBOLS                                                                     \
    T(EndOfFile,    "",         EndOfFileScanner,   NullParser,     BindPower::None)    \
    T(WhiteSpace,   "[ \t]+",   RegexScanner,       NullParser,     BindPower::None)    \
    T(Add,          "+",        LiteralScanner,     InfixParser,    BindPower::Sum)     \
    T(Sub,          "-",        LiteralScanner,     InfixParser,    BindPower::Sum)     \
    T(Mul,          "*",        LiteralScanner,     InfixParser,    BindPower::Product) \
    T(Div,          "/",        LiteralScanner,     InfixParser,    BindPower::Product)  

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

        Symbol();
        Symbol(SymbolType type, std::string pattern, ScanFunc scan, ParseFunc parse, size_t lbp);

        operator bool();
        bool operator==(const Symbol &rhs);
        bool operator!=(const Symbol &rhs);
    };
    */

    class Token {

        Symbol              *_symbol;

        public:
        const std::string   &Source;
        size_t              Index;
        size_t              Length;

        Token();
        Token(Symbol *symbol, const std::string &source, size_t index, size_t length);

        SymbolType Type() const;
        std::string Pattern() const;
        size_t LBP() const;
        size_t Scan(size_t index) const;
        std::string Value() const;
        Ast * Parse(Parser *parser, Ast *ast, Token token) const;
        operator bool();
        friend std::ostream & operator<<(std::ostream &out, const Token &token);
    };

    size_t EndOfFileScanner(const std::string &source, size_t index);
    size_t RegexScanner(const std::string &source, size_t index);
    size_t LiteralScanner(const std::string &source, size_t index);
    Ast * NullParser(Parser *parser, Ast *ast, Token token);
    Ast * InfixParser(Parser *parser, Ast *ast, Token token);

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

#endif /*__SOTA_TOKEN__*/
