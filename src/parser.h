#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <map>
#include <vector>

#include <boost/preprocessor.hpp>

#include "z2h/token.hpp"
#include "z2h/parser.hpp"
#include "z2h/symbol.hpp"
#include "z2h/binder.hpp"

namespace sota {

    class Ast;

    using SotaScan = z2h::ScanFunc<Ast *>;
    using SotaStd = z2h::StdFunc<Ast *>;
    using SotaNud = z2h::NudFunc<Ast *>;
    using SotaLed = z2h::LedFunc<Ast *>;

    using SotaToken = z2h::Token<Ast *>;
    using SotaSymbol = z2h::Symbol<Ast *>;

    struct SotaParser : public z2h::Parser<Ast *, SotaParser> {

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

        //NAME          PATTERN     BINDPOWER               SCANNER             STD                 NUD                 LED                 
        #define SYMBOLS                                                                                                                     \
        T(EndOfFile,    "\0",       BindPower::None,        RegexScanner,       Nullptr,            EndOfFileNud,       Nullptr)            \
        T(WhiteSpace,   "[ \t]+",   BindPower::None,        SkippingScanner,    Nullptr,            WhiteSpaceNud,      Nullptr)            \
        T(Newline,      "[\r\n]+",  BindPower::Sum,         RegexScanner,       Nullptr,            NewlineNud,         Nullptr)            \
        T(Number,       "[0-9]+",   BindPower::None,        RegexScanner,       Nullptr,            NumberNud,          Nullptr)            \
        T(Identifier,   "[a-zA-Z]", BindPower::None,        RegexScanner,       Nullptr,            IdentifierNud,      Nullptr)            \
        T(Colon,        ":",        BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(Equals,       "==",       BindPower::Comparison,  LiteralScanner,     Nullptr,            Nullptr,            ComparisonLed)      \
        T(NotEquals,    "!=",       BindPower::Comparison,  LiteralScanner,     Nullptr,            Nullptr,            ComparisonLed)      \
        T(Add,          "+",        BindPower::Sum,         LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Sub,          "-",        BindPower::Sum,         LiteralScanner,     Nullptr,            PrefixNud,          InfixLed)           \
        T(Mul,          "*",        BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Div,          "/",        BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Mod,          "%",        BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Assign,       "=",        BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(AddAssign,    "+=",       BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(SubAssign,    "-=",       BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(MulAssign,    "*=",       BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(DivAssign,    "/=",       BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(RegexMatch,   "m/",       BindPower::Regex,       LiteralScanner,     Nullptr,            Nullptr,            RegexLed)           \
        T(RegexReplace, "s/",       BindPower::Regex,       LiteralScanner,     Nullptr,            Nullptr,            RegexLed)           \
        T(Question,     "?",        BindPower::Ternary,     LiteralScanner,     Nullptr,            Nullptr,            TernaryLed)         \
        T(If,           "if",       BindPower::Ternary,     LiteralScanner,     Nullptr,            IfThenElifElseNud,  IfThenElseLed)      \
        T(Else,         "else",     BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \

        #define T(k,p,b,s,t,n,l) k,
        enum SymbolType: size_t {
            SYMBOLS
        };
        #undef T

        SotaParser();

    };
}

#endif /*__SOTA_PARSER__*/
