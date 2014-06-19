#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <utility>

#include "utils.h"

//using namespace std;

namespace sota {

    #define META_TOKENS                 \
    T(EndOfFile         ,   "EOF")      \
    T(BegOfFile         ,   "BOF")      \
    T(BegOfZone         ,   "BOZ")      \
    T(EndOfZone         ,   "EOZ")      \
    T(BegOfLine         ,   "BOL")      \
    T(EndOfLine         ,   "EOL")      \
    T(Indent            ,   "INDENT")   \
    T(Dedent            ,   "DEDENT")   \
    T(WhiteSpace        ,   "WS")       \
    T(Comment           ,   "CMT")      \
    T(Id                ,   "ID")       \
    T(Num               ,   "NUM")      \
    T(Str               ,   "STR")      \
    T(Meta              ,   "META")

    #define KEYWORD_TOKENS              \
    T(As                ,   "as")       \
    T(In                ,   "in")       \
    T(Or                ,   "or")       \
    T(Not               ,   "not")      \
    T(If                ,   "if")       \
    T(Elif              ,   "elif")     \
    T(Else              ,   "else")     \
    T(With              ,   "with")     \
    T(While             ,   "while")    \
    T(Try               ,   "try")      \
    T(Catch             ,   "catch")    \
    T(Raise             ,   "raise")    \
    T(Match             ,   "match")    \
    T(Foreach           ,   "foreach")  \
    T(Parallel          ,   "parallel") \
    T(Print             ,   "print")    \
    T(Debug             ,   "debug")    \
    T(Trace             ,   "trace")    \
    T(Async             ,   "async")    \
    T(Await             ,   "await")    \
    T(Yield             ,   "yield")    \
    T(Break             ,   "break")    \
    T(Continue          ,   "continue") \
    T(From              ,   "from")     \
    T(Import            ,   "import")   \
    T(Insert            ,   "insert")   \
    T(This              ,   "this")     \
    T(Base              ,   "base")     \
    T(Null              ,   "null")     \
    T(True              ,   "true")     \
    T(False             ,   "false")

    #define SYMBOL_TOKENS               \
    T(LeftParen         ,   "(")        \
    T(RightParen        ,   ")")        \
    T(LeftBracket       ,   "[")        \
    T(RightBracket      ,   "]")        \
    T(LeftBrace         ,   "{")        \
    T(RightBrace        ,   "}")        \
    T(Backtic           ,   "`")        \
    T(SingleQuote       ,   "\'")       \
    T(DoubleQuote       ,   "\"")       \
    T(Colon             ,   ":")        \
    T(SemiColon         ,   ";")        \
    T(Comma             ,   ",")        \
    T(Tilde             ,   "~")        \
    T(Dot               ,   ".")        \
    T(Updir             ,   "../")      \
    T(RegexMatch        ,   "m/")       \
    T(RegexSubstitute   ,   "s/")       \
    T(Add               ,   "+")        \
    T(Sub               ,   "-")        \
    T(Mul               ,   "*")        \
    T(Div               ,   "/")        \
    T(Modulo            ,   "%")        \
    T(BitOr             ,   "|")        \
    T(BitAnd            ,   "&")        \
    T(GreaterThan       ,   ">")        \
    T(LessThan          ,   "<")        \
    T(Assign            ,   "=")        \
    T(AddAssign         ,   "+=")       \
    T(AssignAdd         ,   "=+")       \
    T(SubAssign         ,   "-=")       \
    T(AssignSub         ,   "=-")       \
    T(MulAssign         ,   "*=")       \
    T(AssignMul         ,   "=*")       \
    T(DivAssign         ,   "/=")       \
    T(AssignDiv         ,   "=/")       \
    T(LogOr             ,   "||")       \
    T(LogAnd            ,   "&&")       \
    T(LogEq             ,   "==")       \
    T(GreaterThanEquals ,   ">=")       \
    T(LessThanEquals    ,   "<=")       \
    T(RightBitShift     ,   ">>")       \
    T(LeftBitShift      ,   "<<")       \
    T(RangeInclusive    ,   "..")       \
    T(RangeExclusive    ,   "...")      \
    T(RightArrow        ,   "->")       \
    T(LeftArrow         ,   "<-")

    #define T(k,v) k,
    enum TokenType {
        META_TOKENS
        KEYWORD_TOKENS
        SYMBOL_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(v, TokenType::k),
    static std::map<std::string, TokenType> Value2Type {
        META_TOKENS
        KEYWORD_TOKENS
        SYMBOL_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(v, TokenType::k),
    static std::map<std::string, TokenType> MetaValue2Type{
        META_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(v, TokenType::k),
    static std::map<std::string, TokenType> KeywordValue2Type {
        KEYWORD_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(v, TokenType::k),
    static std::map<std::string, TokenType> SymbolValue2Type {
        SYMBOL_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(v[0], 1),
    static std::map<char, int> SymbolStart {
        SYMBOL_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(TokenType::k, #k),
    static std::map<TokenType, std::string> Type2Name {
        META_TOKENS
        KEYWORD_TOKENS
        SYMBOL_TOKENS
    };
    #undef T

    #define T(k,v) std::make_pair(TokenType::k, v),
    static std::map<TokenType, std::string> Type2Value {
        META_TOKENS
        KEYWORD_TOKENS
        SYMBOL_TOKENS
    };
    #undef T

    #define T(k,v) v,
    static std::vector<std::string> TokenValues {
        META_TOKENS
        KEYWORD_TOKENS
        SYMBOL_TOKENS
    };
    #undef T

    typedef struct Token {
        TokenType Type;
        unsigned int Index;
        unsigned int Length;
        std::vector<char> Chars;

        operator const TokenType() {
            return Type;
        }

        operator bool() {
            return Type != TokenType::EndOfFile;
        }
        bool operator==(const Token &rhs) {
            return Type == rhs.Type && Index == rhs.Index && Length == rhs.Length;
        }
        bool operator!=(const Token &rhs) {
            return Type != rhs.Type || Index != rhs.Index || Length != rhs.Length;
        }

        std::string Value() const {
            return std::string(Chars.begin() + Index, Chars.begin() + Index + Length);
        }
        friend std::ostream & operator<<(std::ostream &out, const Token &token) {

            std::string result;
            auto value = token.Value();
            auto tokenValue = Type2Value[token.Type];
            switch (token.Type) {
            case TokenType::WhiteSpace:
            case TokenType::EndOfFile:
            case TokenType::EndOfLine:
            case TokenType::Indent:
            case TokenType::Dedent:
                result = tokenValue;
                break;
            default:
                result = value != tokenValue ? tokenValue + ":" + value : value;
                break;
            }
            return out << result;
        }

    } Token;
}

#endif /*__SOTA_TOKEN__*/
