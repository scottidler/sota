#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <map>
#include <vector>
#include "z2h/token.hpp"
#include "z2h/parser.hpp"
#include "z2h/symbol.hpp"

namespace sota {

    class Ast;

    using SotaToken = z2h::Token<Ast *>;
    using SotaSymbol = z2h::Symbol<Ast *>;

    class SotaParser : public z2h::Parser<Ast *> {

    public:

        // must be implemented in derived class (SotaParser)
        std::vector<SotaSymbol *> Symbols();

        // scanners
        long SkippingScanner(SotaSymbol *symbol, const std::string &source, size_t index);
        long RegexScanner(SotaSymbol *symbol, const std::string &source, size_t index);
        long LiteralScanner(SotaSymbol *symbol, const std::string &source, size_t index);

        // std parsing functions
        Ast * NotImplementedStd();

        // nud parsing functions
        Ast * NotImplementedNud(SotaToken *token);
        Ast * EndOfFileNud(SotaToken *token);
        Ast * WhiteSpaceNud(SotaToken *token);
        Ast * NewlineNud(SotaToken *token);
        Ast * NumberNud(SotaToken *token);
        Ast * IdentifierNud(SotaToken *token);
        Ast * PrefixNud(SotaToken *token);
        Ast * IfThenElifElseNud(SotaToken *token);

        // led parsing functions
        Ast * NotImplementedLed(Ast *left, SotaToken *token);
        Ast * ComparisonLed(Ast *left, SotaToken *token);
        Ast * InfixLed(Ast *left, SotaToken *token);
        Ast * PostfixLed(Ast *left, SotaToken *token);
        Ast * AssignLed(Ast *left, SotaToken *token);
        Ast * RegexLed(Ast *left, SotaToken *token);
        Ast * TernaryLed(Ast *left, SotaToken *token);
        Ast * IfThenElseLed(Ast *left, SotaToken *token);

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

    };
}

#endif /*__SOTA_PARSER__*/
