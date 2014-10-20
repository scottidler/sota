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

        std::vector<z2h::Ast *> Expressions(TokenType end, bool leading);

        size_t Update(TokenType type, size_t next) { auto symbol = GetSymbol(type); auto curr = symbol->lbp; symbol->lbp = next; return curr; }

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
        z2h::Ast * EndOfExpressionNud(z2h::Token *token);
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
        z2h::Ast * EndOfExpressionLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * ParensLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * BracesLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * BracketsLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * AssignLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * ArrowLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * CallLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * TernaryLed(z2h::Ast *left, z2h::Token *token);
        z2h::Ast * IfThenElseLed(z2h::Ast *left, z2h::Token *token);

        //NAME                          BINDPOWER                   STD                 NUD                 LED
        #define SYMBOLS                                                                                                         \
        T(TokenType::Eof,               BindPower::None,            Nullptr,            EndOfFileNud,       EndOfFileLed)       \
        T(TokenType::Eos,               BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::Eoe,               BindPower::Separator,       Nullptr,            EndOfExpressionNud, EndOfExpressionLed) \
        T(TokenType::Indent,            BindPower::Denting,         Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::Dedent,            BindPower::Denting,         Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::WhiteSpace,        BindPower::None,            Nullptr,            Nullptr,            Nullptr)            \
        T(TokenType::Arrow,             BindPower::Def,             Nullptr,            Nullptr,            ArrowLed)           \
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

    };
}

#endif /*__SOTA_PARSER__*/
