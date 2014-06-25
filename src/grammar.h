#ifndef __SOTA_GRAMMAR__
#define __SOTA_GRAMMAR__ = 1

#include <map>

namespace sota {

    class Ast;
    class Token;
    class Parser;
    class Symbol;

    // scanners
    size_t RegexScanner(const std::string &source, size_t index);
    size_t LiteralScanner(const std::string &source, size_t index);

    // nud parsing functions
    Ast * NotImplementedNud(Parser *parser, Token *token);
    Ast * EndOfFileNud(Parser *parser, Token *token);
    Ast * WhiteSpaceNud(Parser *parser, Token *token);
    Ast * IdentifierNud(Parser *parser, Token *token);
    Ast * PrefixOperatorNud(Parser *parser, Token *token);

    // led parsing functions
    Ast * NotImplementedLed(Parser *parser, Ast *left, Token *token);
    Ast * InfixOperatorLed(Parser *parser, Ast *left, Token *token);
    Ast * PostfixOperatorLed(Parser *parser, Ast *left, Token *token);

    //NAME          PATTERN     SCANNER             NUD                 LED                 LEFT_BIND_POWER
    #define SYMBOLS                                                                                                 \
    T(EndOfFile,    "\0",       LiteralScanner,     EndOfFileNud,       NotImplementedLed,  BindPower::None)        \
    T(WhiteSpace,   "[ \t]+",   RegexScanner,       WhiteSpaceNud,      NotImplementedLed,  BindPower::None)        \
    T(Identifier,   "[a-zA-Z]", RegexScanner,       IdentifierNud,      NotImplementedLed,  BindPower::None)        \
    T(Add,          "+",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Sum)         \
    T(Sub,          "-",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Sum)         \
    T(Mul,          "*",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Product)     \
    T(Div,          "/",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Product)     \
    T(Neg,          "-",        LiteralScanner,     PrefixOperatorNud,  NotImplementedLed,  BindPower::Negation)

    #define T(k,p,s,n,l,b) k,
    enum SymbolType: size_t {
        SYMBOLS
    };
    #undef T

    extern std::map<size_t, Symbol *> Type2Symbol;
    extern std::map<std::string, Symbol *> Name2Symbol;
    extern std::map<size_t, std::string> Type2Name;
    extern std::map<std::string, size_t> Name2Type;
}

#endif /*__SOTA_GRAMMAR__*/
