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
#include "stack.hpp"

namespace sota {

    class Ast;
    class Token;
    class SotaLexer;

    struct SotaParser : public z2h::Parser<SotaParser> {

        SotaStack<size_t> denting;
        SotaStack<z2h::Token *> nesting;

        SotaParser(SotaLexer *lexer);

        z2h::Ast * Parse();
        std::vector<z2h::Symbol *> Symbols();
        std::vector<z2h::Token *> Tokenize();

        std::vector<z2h::Ast *> Expressions(z2h::Token *end);

        // must be implemented in derived class (SotaParser)
        std::exception Exception(const char *file, size_t line, const std::string &message = "");

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
        z2h::Ast * RegexMatchNud(z2h::Token *token);
        z2h::Ast * RegexReplaceNud(z2h::Token *token);
        z2h::Ast * IfThenElifElseNud(z2h::Token *token);

        // led parsing functions
        z2h::Ast * NullptrLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * EndOfFileLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * ComparisonLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * InfixLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * PostfixLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * CommaLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * ParensLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * BracesLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * BracketsLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * AssignLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * FuncLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * CallLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * TernaryLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * IfThenElseLed(z2h::Ast *left, z2h::Token *token);
/*
        //                  4                   3                       6                   5                   5                   5
        //NAME              PATTERN             BINDPOWER               SCANNER             STD                 NUD                 LED
        #define SYMBOLS                                                                                                                             \
        T(EndOfFile,        "\0",               BindPower::None,        RegexScanner,       Nullptr,            EndOfFileNud,       EndOfFileLed)       \
        T(EndOfStatement,   "[\r\n]+|;",        BindPower::None,        EosScanner,         Nullptr,            Nullptr,            Nullptr)            \
        T(EndOfExpression,  ",([ \t]*,)*",      BindPower::Separator,   RegexScanner,       Nullptr,            CommaNud,           CommaLed)           \
        T(Indent,           "[\r\n]+[ \t]+",    BindPower::Denting,     DentingScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(Dedent,           "[\r\n]+[ \t]*",    BindPower::Denting,     DentingScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(WhiteSpace,       "[ \t]+",           BindPower::None,        SkippingScanner,    Nullptr,            Nullptr,            Nullptr)            \
        T(Arrow,            "->",               BindPower::Func,        LiteralScanner,     Nullptr,            Nullptr,            FuncLed)            \
        T(Number,           "[0-9]+",           BindPower::None,        RegexScanner,       Nullptr,            NumberNud,          Nullptr)            \
        T(Identifier,       "([0-9]+)?[a-zA-Z]+([a-zA-Z0-9]+)?",    BindPower::None,        RegexScanner,       Nullptr,            IdentifierNud,      Nullptr)            \
        T(Colon,            ":",                BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(LeftParen,        "(",                BindPower::Group,       LiteralScanner,     Nullptr,            ParensNud,          ParensLed)          \
        T(RightParen,       ")",                BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(LeftBrace,        "{",                BindPower::Group,       LiteralScanner,     Nullptr,            BracesNud,          BracesLed)          \
        T(RightBrace,       "}",                BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(LeftBracket,      "[",                BindPower::Group,       LiteralScanner,     Nullptr,            BracketsNud,        BracketsLed)        \
        T(RightBracket,     "]",                BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
        T(Equals,           "==",               BindPower::Comparison,  LiteralScanner,     Nullptr,            Nullptr,            ComparisonLed)      \
        T(NotEquals,        "!=",               BindPower::Comparison,  LiteralScanner,     Nullptr,            Nullptr,            ComparisonLed)      \
        T(Add,              "+",                BindPower::Sum,         LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Sub,              "-",                BindPower::Sum,         LiteralScanner,     Nullptr,            PrefixNud,          InfixLed)           \
        T(Mul,              "*",                BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Div,              "/",                BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Mod,              "%",                BindPower::Product,     LiteralScanner,     Nullptr,            Nullptr,            InfixLed)           \
        T(Assign,           "=",                BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(AddAssign,        "+=",               BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(SubAssign,        "-=",               BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(MulAssign,        "*=",               BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(DivAssign,        "/=",               BindPower::Assignment,  LiteralScanner,     Nullptr,            Nullptr,            AssignLed)          \
        T(RegexMatch,       "m/",               BindPower::Regex,       LiteralScanner,     Nullptr,            RegexMatchNud,      Nullptr)            \
        T(RegexReplace,     "s/",               BindPower::Regex,       LiteralScanner,     Nullptr,            RegexReplaceNud,    Nullptr)            \
        T(Question,         "?",                BindPower::Ternary,     LiteralScanner,     Nullptr,            Nullptr,            TernaryLed)         \
        T(If,               "if",               BindPower::Ternary,     LiteralScanner,     Nullptr,            IfThenElifElseNud,  IfThenElseLed)      \
        T(Else,             "else",             BindPower::None,        LiteralScanner,     Nullptr,            Nullptr,            Nullptr)            \
*/
        //                  4                   3                                           5                   5                   5
        //NAME              PATTERN             BINDPOWER                                   STD                 NUD                 LED
        #define SYMBOLS                                                                                                                                 \
        T(TokenType::EndOfFile,         BindPower::None,            Nullptr,            EndOfFileNud,       EndOfFileLed)       \
        T(TokenType::EndOfStatement,    BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::EndOfExpression,   BindPower::Separator,       Nullptr,            CommaNud,           CommaLed)           \
        T(TokenType::Indent,            BindPower::Denting,         Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::Dedent,            BindPower::Denting,         Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::WhiteSpace,        BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::Arrow,             BindPower::Func,            Nullptr,            Nullptr,            FuncLed)            \
        T(TokenType::Number,            BindPower::None,            Nullptr,            NumberNud,          Nullptr)            \
        T(TokenType::Identifier,        BindPower::None,            Nullptr,            IdentifierNud,      Nullptr)            \
        T(TokenType::Colon,             BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::LeftParen,         BindPower::Group,           Nullptr,            ParensNud,          ParensLed)          \
        T(TokenType::RightParen,        BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::LeftBrace,         BindPower::Group,           Nullptr,            BracesNud,          BracesLed)          \
        T(TokenType::RightBrace,        BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::LeftBracket,       BindPower::Group,           Nullptr,            BracketsNud,        BracketsLed)        \
        T(TokenType::RightBracket,      BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::Equals,            BindPower::Comparison,      Nullptr,            Nullptr,            ComparisonLed)      \
        T(TokenType::NotEquals,         BindPower::Comparison,      Nullptr,            Nullptr,            ComparisonLed)      \
        T(TokenType::Add,               BindPower::Sum,             Nullptr,            Nullptr,            InfixLed)           \
        T(TokenType::Sub,               BindPower::Sum,             Nullptr,            PrefixNud,          InfixLed)           \
        T(TokenType::Mul,               BindPower::Product,         Nullptr,            Nullptr,            InfixLed)           \
        T(TokenType::Div,               BindPower::Product,         Nullptr,            Nullptr,            InfixLed)           \
        T(TokenType::Mod,               BindPower::Product,         Nullptr,            Nullptr,            InfixLed)           \
        T(TokenType::Assign,            BindPower::Assignment,      Nullptr,            Nullptr,            AssignLed)          \
        T(TokenType::AddAssign,         BindPower::Assignment,      Nullptr,            Nullptr,            AssignLed)          \
        T(TokenType::SubAssign,         BindPower::Assignment,      Nullptr,            Nullptr,            AssignLed)          \
        T(TokenType::MulAssign,         BindPower::Assignment,      Nullptr,            Nullptr,            AssignLed)          \
        T(TokenType::DivAssign,         BindPower::Assignment,      Nullptr,            Nullptr,            AssignLed)          \
        T(TokenType::RegexMatch,        BindPower::Regex,           Nullptr,            RegexMatchNud,      Nullptr)            \
        T(TokenType::RegexReplace,      BindPower::Regex,           Nullptr,            RegexReplaceNud,    Nullptr)            \
        T(TokenType::Question,          BindPower::Ternary,         Nullptr,            Nullptr,            TernaryLed)         \
        T(TokenType::If,                BindPower::Ternary,         Nullptr,            IfThenElifElseNud,  IfThenElseLed)      \
        T(TokenType::Else,              BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
/*
        #define T(k,b,t,n,l) k,
        enum TokenType: size_t {
            SYMBOLS
        };
        #undef T
        std::vector<SotaSymbol *> symbols;
        //std::map<TokenType, z2h::Symbol *> symbolmap;
*/
    };
}

#endif /*__SOTA_PARSER__*/
