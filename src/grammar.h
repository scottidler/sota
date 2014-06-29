#ifndef __SOTA_GRAMMAR__
#define __SOTA_GRAMMAR__ = 1

#include <map>

namespace sota {

    class Ast;
    class Token;
    class Parser;
    class Symbol;

    // scanners
    long SkippingScanner(Symbol *symbol, const std::string &source, size_t index);
    long RegexScanner(Symbol *symbol, const std::string &source, size_t index);
    long LiteralScanner(Symbol *symbol, const std::string &source, size_t index);

    // nud parsing functions
    Ast * NotImplementedNud(Parser *parser, Token *token);
    Ast * EndOfFileNud(Parser *parser, Token *token);
    Ast * WhiteSpaceNud(Parser *parser, Token *token);
    Ast * NewlineNud(Parser *parser, Token *token);
    Ast * NumberNud(Parser *parser, Token *token);
    Ast * IdentifierNud(Parser *parser, Token *token);
    Ast * PrefixOperatorNud(Parser *parser, Token *token);

    // led parsing functions
    Ast * NotImplementedLed(Parser *parser, Ast *left, Token *token);
    Ast * InfixOperatorLed(Parser *parser, Ast *left, Token *token);
    Ast * PostfixOperatorLed(Parser *parser, Ast *left, Token *token);

    //NAME          PATTERN     SCANNER             NUD                 LED                 LEFT_BIND_POWER
    #define SYMBOLS                                                                                                 \
    T(EndOfFile,    "\0",       RegexScanner,       EndOfFileNud,       NotImplementedLed,  BindPower::None)        \
    T(WhiteSpace,   "[ \t]+",   SkippingScanner,    WhiteSpaceNud,      NotImplementedLed,  BindPower::None)        \
    T(Newline,      "[\r\n]+",  RegexScanner,       NewlineNud,         NotImplementedLed,  BindPower::Sum)         \
    T(Number,       "[0-9]+",   RegexScanner,       NumberNud,          NotImplementedLed,  BindPower::None)        \
    T(Identifier,   "[a-zA-Z]", RegexScanner,       IdentifierNud,      NotImplementedLed,  BindPower::None)        \
    T(Add,          "+",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Sum)         \
    T(Sub,          "-",        LiteralScanner,     PrefixOperatorNud,  InfixOperatorLed,   BindPower::Sum)         \
    T(Mul,          "*",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Product)     \
    T(Div,          "/",        LiteralScanner,     NotImplementedNud,  InfixOperatorLed,   BindPower::Product)     \

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
