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
    Ast * PrefixNud(Parser *parser, Token *token);
    Ast * IfThenElifElseNud(Parser *parser, Token *token);

    // led parsing functions
    Ast * NotImplementedLed(Parser *parser, Ast *left, Token *token);
    Ast * ComparisonLed(Parser *parser, Ast *left, Token *token);
    Ast * InfixLed(Parser *parser, Ast *left, Token *token);
    Ast * PostfixLed(Parser *parser, Ast *left, Token *token);
    Ast * AssignLed(Parser *parser, Ast *left, Token *token);
    Ast * RegexLed(Parser *parser, Ast *left, Token *token);
    Ast * TernaryLed(Parser *parser, Ast *left, Token *token);
    Ast * IfThenElseLed(Parser *parser, Ast *left, Token *token);

    //NAME          PATTERN     SCANNER             NUD                 LED                 BINDPOWER
    #define SYMBOLS                                                                                                 \
    T(EndOfFile,    "\0",       RegexScanner,       EndOfFileNud,       NotImplementedLed,  BindPower::None)        \
    T(WhiteSpace,   "[ \t]+",   SkippingScanner,    WhiteSpaceNud,      NotImplementedLed,  BindPower::None)        \
    T(Newline,      "[\r\n]+",  RegexScanner,       NewlineNud,         NotImplementedLed,  BindPower::Sum)         \
    T(Number,       "[0-9]+",   RegexScanner,       NumberNud,          NotImplementedLed,  BindPower::None)        \
    T(Identifier,   "[a-zA-Z]", RegexScanner,       IdentifierNud,      NotImplementedLed,  BindPower::None)        \
    T(Colon,        ":",        LiteralScanner,     NotImplementedNud,  NotImplementedLed,  BindPower::None)        \
    T(Equals,       "==",       LiteralScanner,     NotImplementedNud,  ComparisonLed,      BindPower::Comparison)  \
    T(NotEquals,    "!=",       LiteralScanner,     NotImplementedNud,  ComparisonLed,      BindPower::Comparison)  \
    T(Add,          "+",        LiteralScanner,     NotImplementedNud,  InfixLed,           BindPower::Sum)         \
    T(Sub,          "-",        LiteralScanner,     PrefixNud,          InfixLed,           BindPower::Sum)         \
    T(Mul,          "*",        LiteralScanner,     NotImplementedNud,  InfixLed,           BindPower::Product)     \
    T(Div,          "/",        LiteralScanner,     NotImplementedNud,  InfixLed,           BindPower::Product)     \
    T(Mod,          "%",        LiteralScanner,     NotImplementedNud,  InfixLed,           BindPower::Product)     \
    T(Assign,       "=",        LiteralScanner,     NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
    T(AddAssign,    "+=",       LiteralScanner,     NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
    T(SubAssign,    "-=",       LiteralScanner,     NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
    T(MulAssign,    "*=",       LiteralScanner,     NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
    T(DivAssign,    "/=",       LiteralScanner,     NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
    T(RegexMatch,   "m/",       LiteralScanner,     NotImplementedNud,  RegexLed,           BindPower::Regex)       \
    T(RegexReplace, "s/",       LiteralScanner,     NotImplementedNud,  RegexLed,           BindPower::Regex)       \
    T(Question,     "?",        LiteralScanner,     NotImplementedNud,  TernaryLed,         BindPower::Ternary)     \
    T(If,           "if",       LiteralScanner,     IfThenElifElseNud,  IfThenElseLed,      BindPower::Ternary)     \
    T(Else,         "else",     LiteralScanner,     NotImplementedNud,  NotImplementedLed,  BindPower::None)        \

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
