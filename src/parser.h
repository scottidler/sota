#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#define SCAN(x) BindScan(&SotaParser::x)
#define STD(x) BindStd(&SotaParser::x)
#define NUD(x) BindNud(&SotaParser::x)
#define LED(x) BindLed(&SotaParser::x)

#include <map>
#include <stack>
#include <vector>

#include <boost/preprocessor.hpp>

#include "z2h/parser.hpp"

namespace sota {

    class Ast;
    class Symbol;
    class Token;

    struct SotaParser : public z2h::Parser<SotaParser> {

        std::stack<z2h::Token *> nesting;

        SotaParser();

        std::vector<z2h::Token *> TokenizeFile(const std::string &filename);
        std::vector<z2h::Token *> Tokenize(std::string source);
        std::vector<z2h::Ast *> ParseFile(const std::string &filename);
        std::vector<z2h::Ast *> Parse(std::string source);

        std::vector<z2h::Ast *> Expressions(z2h::Symbol *end);
        //std::vector<z2h::Ast *> Statements(z2h::Symbol *end);

        // must be implemented in derived class (SotaParser)
        std::exception Exception(const char *file, size_t line, const std::string &message = "");
        std::vector<z2h::Symbol *> Symbols();

        // scanners
        z2h::Token * SkippingScanner(z2h::Symbol *symbol, const std::string &source, size_t position);
        z2h::Token * RegexScanner(z2h::Symbol *symbol, const std::string &source, size_t position);
        z2h::Token * LiteralScanner(z2h::Symbol *symbol, const std::string &source, size_t position);
        z2h::Token * EosScanner(z2h::Symbol *symbol, const std::string &source, size_t position);
        z2h::Token * EoeScanner(z2h::Symbol *symbol, const std::string &source, size_t position);
        z2h::Token * DentingScanner(z2h::Symbol *symbol, const std::string &source, size_t position);

        // std parsing functions
        z2h::Ast * NullptrStd();

        // nud parsing functions
        z2h::Ast * NullptrNud(z2h::Token *token);
        z2h::Ast * EndOfFileNud(z2h::Token *token);
        z2h::Ast * NewlineNud(z2h::Token *token);
        z2h::Ast * NumberNud(z2h::Token *token);
        z2h::Ast * IdentifierNud(z2h::Token *token);
        z2h::Ast * PrefixNud(z2h::Token *token);
        z2h::Ast * CommaNud(z2h::Token *token);
        z2h::Ast * ParensNud(z2h::Token *token);
        z2h::Ast * BracesNud(z2h::Token *token);
        z2h::Ast * BracketsNud(z2h::Token *token);
        z2h::Ast * IfThenElifElseNud(z2h::Token *token);

        // led parsing functions
        z2h::Ast * NullptrLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * EndOfFileLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * ComparisonLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * InfixLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * PostfixLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * CommaLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * AssignLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * FuncLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * RegexLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * CallLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * TernaryLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * IfThenElseLed(z2h::Ast *left, z2h::Token *token);

        //              4                   3                       6                   5                   5                   5
        //NAME          PATTERN             BINDPOWER               SCANNER             STD                 NUD                 LED
        #define SYMBOLS                                                                                                                             \
        T(EndOfFile,        "\0",           BindPower::None,        RegexScanner,       Nullptr,            EndOfFileNud,       EndOfFileLed)       \
        T(EndOfStatement,   "[\r\n]+|;",    BindPower::None,        EosScanner,         Nullptr,            Nullptr,            Nullptr)            \
        T(EndOfExpression,  ",",            BindPower::Separator,   RegexScanner,       Nullptr,            CommaNud,           CommaLed)           \
        T(Indent,           "[\r\n]+\\s+",  BindPower::Denting,     DentingScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(Dedent,           "[\r\n]+\\s+",  BindPower::Denting,     DentingScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(WhiteSpace,       "[ \t]+",       BindPower::None,        SkippingScanner,    Nullptr,            Nullptr,            Nullptr)            \
        T(Arrow,            "->",           BindPower::Func,        LiteralScanner,     Nullptr,            Nullptr,            FuncLed)            \
        T(Number,           "[0-9]+",       BindPower::None,        RegexScanner,       Nullptr,            NumberNud,          Nullptr)            \
        T(Identifier,       "([0-9]+)?[a-zA-Z]+([a-zA-Z0-9]+)?",    BindPower::None,        RegexScanner,       Nullptr,            IdentifierNud,      Nullptr)            \
        T(Colon,            ":",            BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(LeftParen,        "(",            BindPower::Group,       LiteralScanner,     Nullptr,            ParensNud,          Nullptr)            \
        T(RightParen,       ")",            BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(LeftBrace,        "{",            BindPower::Group,       LiteralScanner,     Nullptr,            BracesNud,          Nullptr)            \
        T(RightBrace,       "}",            BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(LeftBracket,      "[",            BindPower::Group,       LiteralScanner,     Nullptr,            BracketsNud,        Nullptr)            \
        T(RightBracket,     "]",            BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(Equals,           "==",           BindPower::Comparison,  LiteralScanner,     Nullptr,            Nullptr,            ComparisonLed)      \
        T(NotEquals,        "!=",           BindPower::Comparison,  LiteralScanner,     Nullptr,            Nullptr,            ComparisonLed)      \
        T(Add,              "+",            BindPower::Sum,         LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Sub,              "-",            BindPower::Sum,         LiteralScanner,     Nullptr,            PrefixNud,          InfixLed)           \
        T(Mul,              "*",            BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Div,              "/",            BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Mod,              "%",            BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Assign,           "=",            BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(AddAssign,        "+=",           BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(SubAssign,        "-=",           BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(MulAssign,        "*=",           BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(DivAssign,        "/=",           BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(RegexMatch,       "m/",           BindPower::Regex,       LiteralScanner,     Nullptr,            Nullptr,            RegexLed)           \
        T(RegexReplace,     "s/",           BindPower::Regex,       LiteralScanner,     Nullptr,            Nullptr,            RegexLed)           \
        T(Question,         "?",            BindPower::Ternary,     LiteralScanner,     Nullptr,            Nullptr,            TernaryLed)         \
        T(If,               "if",           BindPower::Ternary,     LiteralScanner,     Nullptr,            IfThenElifElseNud,  IfThenElseLed)      \
        T(Else,             "else",         BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \

        #define T(k,p,b,s,t,n,l) k,
        enum SymbolType: size_t {
            SYMBOLS
        };
        #undef T

        std::map<SymbolType, z2h::Symbol *> symbolmap;

    };
}

#endif /*__SOTA_PARSER__*/
