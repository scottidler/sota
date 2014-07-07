#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <map>
#include <vector>

#include <boost/preprocessor.hpp>

#include "z2h/token.hpp"
#include "z2h/parser.hpp"
#include "z2h/symbol.hpp"

#define BIND0(method) (std::bind(method, this))
#define BIND1(method) (std::bind(method, this, std::placeholders::_1))
#define BIND2(method) (std::bind(method, this, std::placeholders::_1, std::placeholders::_2))
#define BIND3(method) (std::bind(method, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))

#define STD_IF(method) (method == nullptr) ? ((SotaStd)nullptr) : ((SotaStd)std::bind(method, this))
#define NUD_IF(method) (method == nullptr) ? ((SotaLed)nullptr) : ((SotaLed)std::bind(method, this, std::placeholders::_1))
#define LED_IF(method) (method == nullptr) ? ((SotaLed)nullptr) : ((SotaLed)std::bind(method, this, std::placeholders::_1, std::placeholders::_2))
#define SCAN_IF(method) (method == nullptr) ? ((SotaLed)nullptr) : ((SotaLed)std::bind(method, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))

namespace sota {

    class Ast;

    using SotaScan = z2h::ScanFunc<Ast *>;
    using SotaStd = z2h::StdFunc<Ast *>;
    using SotaNud = z2h::NudFunc<Ast *>;
    using SotaLed = z2h::LedFunc<Ast *>;

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

        //NAME          PATTERN     BINDPOWER               SCANNER             STD                 NUD                 LED                 
        #define SYMBOLS                                                                                                                     \
        T(EndOfFile,    "\0",       BindPower::None,        RegexScanner,       NotImplementedStd,  EndOfFileNud,       NotImplementedLed)  \
        T(WhiteSpace,   "[ \t]+",   BindPower::None,        SkippingScanner,    NotImplementedStd,  WhiteSpaceNud,      NotImplementedLed)  \
        T(Newline,      "[\r\n]+",  BindPower::Sum,         RegexScanner,       NotImplementedStd,  NewlineNud,         NotImplementedLed)  \
        T(Number,       "[0-9]+",   BindPower::None,        RegexScanner,       NotImplementedStd,  NumberNud,          NotImplementedLed)  \
        T(Identifier,   "[a-zA-Z]", BindPower::None,        RegexScanner,       NotImplementedStd,  IdentifierNud,      NotImplementedLed)  \
        T(Colon,        ":",        BindPower::None,        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  NotImplementedLed)  \
        T(Equals,       "==",       BindPower::Comparison,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  ComparisonLed)      \
        T(NotEquals,    "!=",       BindPower::Comparison,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  ComparisonLed)      \
        T(Add,          "+",        BindPower::Sum,         LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed)           \
        T(Sub,          "-",        BindPower::Sum,         LiteralScanner,     NotImplementedStd,  PrefixNud,          InfixLed)           \
        T(Mul,          "*",        BindPower::Product,     LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed)           \
        T(Div,          "/",        BindPower::Product,     LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed)           \
        T(Mod,          "%",        BindPower::Product,     LiteralScanner,     NotImplementedStd,  NotImplementedNud,  InfixLed)           \
        T(Assign,       "=",        BindPower::Assignment,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed)          \
        T(AddAssign,    "+=",       BindPower::Assignment,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed)          \
        T(SubAssign,    "-=",       BindPower::Assignment,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed)          \
        T(MulAssign,    "*=",       BindPower::Assignment,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed)          \
        T(DivAssign,    "/=",       BindPower::Assignment,  LiteralScanner,     NotImplementedStd,  NotImplementedNud,  AssignLed)          \
        T(RegexMatch,   "m/",       BindPower::Regex,       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  RegexLed)           \
        T(RegexReplace, "s/",       BindPower::Regex,       LiteralScanner,     NotImplementedStd,  NotImplementedNud,  RegexLed)           \
        T(Question,     "?",        BindPower::Ternary,     LiteralScanner,     NotImplementedStd,  NotImplementedNud,  TernaryLed)         \
        T(If,           "if",       BindPower::Ternary,     LiteralScanner,     NotImplementedStd,  IfThenElifElseNud,  IfThenElseLed)      \
        T(Else,         "else",     BindPower::None,        LiteralScanner,     NotImplementedStd,  NotImplementedNud,  NotImplementedLed)  \

        #define T(k,p,b,s,t,n,l) k,
        enum SymbolType: size_t {
            SYMBOLS
        };
        #undef T

        SotaParser();

    };
}

#endif /*__SOTA_PARSER__*/
