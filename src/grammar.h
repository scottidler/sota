#ifndef __SOTA_GRAMMAR__
#define __SOTA_GRAMMAR__ = 1

#include <map>

namespace sota {

    class Symbol;

    //NAME          PATTERN     SCANNER             PARSER          LEFT_BIND_POWER
    #define SYMBOLS                                                                     \
    T(EndOfFile,    "",         EndOfFileScanner,   NullParser,     BindPower::None)    \
    T(WhiteSpace,   "[ \t]+",   RegexScanner,       NullParser,     BindPower::None)    \
    T(Add,          "+",        LiteralScanner,     InfixParser,    BindPower::Sum)     \
    T(Sub,          "-",        LiteralScanner,     InfixParser,    BindPower::Sum)     \
    T(Mul,          "*",        LiteralScanner,     InfixParser,    BindPower::Product) \
    T(Div,          "/",        LiteralScanner,     InfixParser,    BindPower::Product)  

    #define T(n,v,s,p,b) n,
    enum SymbolType: size_t {
        SYMBOLS
    };
    #undef T

    /*
    extern std::map<SymbolType, Symbol *> Type2Symbol;
    extern std::map<std::string, Symbol *> Name2Symbol; 
    */
}

#endif /*__SOTA_GRAMMAR__*/
