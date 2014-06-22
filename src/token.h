#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <string>
#include <functional>

#include "ast.h"
#include "parser.h"

namespace sota {

    enum BindPower: size_t {
        None = 0,
        Sum = 10,
        Product = 20,
    };

    #define SYMBOLS                                                                     \
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

    class Ast;
    class Token;
    class Parser;
    class Symbol {

        public:
        typedef std::function<size_t(const std::string &, size_t)> ScanFunc;
        typedef std::function<Ast*(Parser*, Ast*, Token)> ParseFunc;

        SymbolType      Type;
        std::string     Name;
        size_t          LBP;
        ScanFunc        Scan;
        ParseFunc       Parse;

        Symbol(SymbolType type, std::string name, ScanFunc scan, ParseFunc parse, size_t lbp) {
            Type = type;
            Name = name;
            Scan = scan;
            Parse = parse;
            LBP = lbp; 
        }
    };

    class Token {

        Symbol              *_symbol;

        public:
        const std::string   &Source;
        size_t              Index;
        size_t              Length;

        Token(Symbol *symbol, const std::string &source, size_t index, size_t length): 
            _symbol(symbol), Source(source), Index(index), Length(length) {
        }

        SymbolType Type() {
            return _symbol->Type;
        }

        std::string Name() {
            return _symbol->Name;
        }

        size_t LBP() {
            return _symbol->LBP;
        }

        size_t Scan(size_t index) {
            return _symbol->Scan(Source, index);
        }

        Ast * Parse(Parser *parser, Ast *ast, Token token) {
            return _symbol->Parse(parser, ast, token);
        }
    };
}

#endif /*__SOTA_TOKEN__*/
