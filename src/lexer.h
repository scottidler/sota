#ifndef __SOTA_LEXER__
#define __SOTA_LEXER__ = 1

#include <map>
#include <vector>

#include "z2h/token.hpp"
#include "z2h/symbol.hpp"

namespace sota {

    #define TOKENS                                              \
    T(Eof,              "\0")                                   \
    T(Eos,              "re:[\r\n]+|;")                         \
    T(Eoe,              "re:([ \t]*,)*")                        \
    T(Indent,           "re:[\r\n]+[ \t]+")                     \
    T(Dedent,           "re:[\r\n]+[ \t]*")                     \
    T(WhiteSpace,       "re:[ \t]+")                            \
    T(Arrow,            "->")                                   \
    T(Number,           "re:[0-9]+")                            \
    T(Identifier,       "re:([0-9]+)?[a-zA-Z]+([a-zA-Z0-9]+)?") \
    T(Colon,            ":")                                    \
    T(LeftParen,        "(")                                    \
    T(RightParen,       ")")                                    \
    T(LeftBrace,        "{")                                    \
    T(RightBrace,       "}")                                    \
    T(LeftBracket,      "[")                                    \
    T(RightBracket,     "]")                                    \
    T(Equals,           "==")                                   \
    T(NotEquals,        "!=")                                   \
    T(Add,              "+")                                    \
    T(Sub,              "-")                                    \
    T(Mul,              "*")                                    \
    T(Div,              "/")                                    \
    T(Mod,              "%")                                    \
    T(Assign,           "=")                                    \
    T(AddAssign,        "+=")                                   \
    T(SubAssign,        "-=")                                   \
    T(MulAssign,        "*=")                                   \
    T(DivAssign,        "/=")                                   \
    T(RegexMatch,       "m/")                                   \
    T(RegexReplace,     "s/")                                   \
    T(Question,         "?")                                    \
    T(If,               "if")                                   \
    T(Else,             "else")                                 \

    #define T(t,p) t,
    enum TokenType: size_t {
        TOKENS
    };
    #undef T

    struct SotaLexer : public z2h::Lexer {

        std::map<TokenType, std::string> tokenmap;

        SotaLexer(std::string source);

        bool IsRegex(std::string &pattern);
        z2h::Token * LiteralScan(size_t type, const std::string &pattern, const std::string &source, size_t position);
        z2h::Token * RegexScan(size_t type, const std::string &prefixed_pattern, const std::string &source, size_t position);

        z2h::Token * Scan();
        std::vector<z2h::Token *> Tokenize();
    };

}

#endif
