#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <map>
#include <vector>
#include "z2h/token.hpp"
#include "z2h/parser.hpp"
#include "z2h/symbol.hpp"

namespace sota {

    class Ast;

    class SotaParser : public z2h::Parser<Ast *> {

    public:
        // scanners
        long SkippingScanner(z2h::Symbol<Ast *> *symbol, const std::string &source, size_t index);
        long RegexScanner(z2h::Symbol<Ast *> *symbol, const std::string &source, size_t index);
        long LiteralScanner(z2h::Symbol<Ast *> *symbol, const std::string &source, size_t index);

        // std parsing functions
        Ast * NotImplementedStd(z2h::Parser<Ast *> *parser);

        // nud parsing functions
        Ast * NotImplementedNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * EndOfFileNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * WhiteSpaceNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * NewlineNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * NumberNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * IdentifierNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * PrefixNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);
        Ast * IfThenElifElseNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token);

        // led parsing functions
        Ast * NotImplementedLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * ComparisonLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * InfixLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * PostfixLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * AssignLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * RegexLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * TernaryLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);
        Ast * IfThenElseLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token);

        //NAME          PATTERN     SCANNER             STD                 NUD                 LED                 BINDPOWER
        #define SYMBOLS                                                                                                                     \
        T(EndOfFile,    "\0",       RegexScanner,       NotImplementedStd,  EndOfFileNud,       NotImplementedLed,  BindPower::None)        \
        T(WhiteSpace,   "[ \t]+",   SkippingScanner,    NotImplementedStd,  WhiteSpaceNud,      NotImplementedLed,  BindPower::None)        \
        T(Newline,      "[\r\n]+",  RegexScanner,       NotImplementedStd,  NewlineNud,         NotImplementedLed,  BindPower::Sum)         \
        T(Number,       "[0-9]+",   RegexScanner,       NotImplementedStd,  NumberNud,          NotImplementedLed,  BindPower::None)        \
        T(Identifier,   "[a-zA-Z]", RegexScanner,       NotImplementedStd,  IdentifierNud,      NotImplementedLed,  BindPower::None)        \
        T(Colon,        ":",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  NotImplementedLed,  BindPower::None)        \
        T(Equals,       "==",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  ComparisonLed,      BindPower::Comparison)  \
        T(NotEquals,    "!=",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  ComparisonLed,      BindPower::Comparison)  \
        T(Add,          "+",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed,           BindPower::Sum)         \
        T(Sub,          "-",        LiteralScanner,     NotImplementedStd,  PrefixNud,          InfixLed,           BindPower::Sum)         \
        T(Mul,          "*",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed,           BindPower::Product)     \
        T(Div,          "/",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed,           BindPower::Product)     \
        T(Mod,          "%",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed,           BindPower::Product)     \
        T(Assign,       "=",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
        T(AddAssign,    "+=",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
        T(SubAssign,    "-=",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
        T(MulAssign,    "*=",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
        T(DivAssign,    "/=",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed,          BindPower::Assignment)  \
        T(RegexMatch,   "m/",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  RegexLed,           BindPower::Regex)       \
        T(RegexReplace, "s/",       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  RegexLed,           BindPower::Regex)       \
        T(Question,     "?",        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  TernaryLed,         BindPower::Ternary)     \
        T(If,           "if",       LiteralScanner,     NotImplementedStd,  IfThenElifElseNud,  IfThenElseLed,      BindPower::Ternary)     \
        T(Else,         "else",     LiteralScanner,     NotImplementedStd,  NotImplementedNud,  NotImplementedLed,  BindPower::None)        \

        #define T(k,p,s,t,n,l,b) k,
        enum SymbolType: size_t {
            SYMBOLS
        };
        #undef T

        SotaParser();
        std::vector<z2h::Symbol<Ast *> *> Symbols();

    };
}

#endif /*__SOTA_PARSER__*/
