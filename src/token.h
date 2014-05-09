#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <xutility>

#include "utils.h"

using namespace std;

namespace sota {
    namespace lexer {

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
        T(Comment           ,   "COMMENT")  \
        T(Id                ,   "ID")       \
        T(Num               ,   "NUM")      \
        T(Str               ,   "STR")

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

        #define T(k,v) make_pair(v, TokenType::k),
        static map<string, TokenType> Value2Type {
            META_TOKENS
            KEYWORD_TOKENS
            SYMBOL_TOKENS
        };
        #undef T

        #define T(k,v) make_pair(v, TokenType::k),
        static map<string, TokenType> MetaValue2Type{
            META_TOKENS
        };
        #undef T

        #define T(k,v) make_pair(v, TokenType::k),
        static map<string, TokenType> KeywordValue2Type {
            KEYWORD_TOKENS
        };
        #undef T

        #define T(k,v) make_pair(v, TokenType::k),
        static map<string, TokenType> SymbolValue2Type {
            SYMBOL_TOKENS
        };
        #undef T

        #define T(k,v) make_pair(v[0], 1),
        static map<char, int> SymbolStart {
            SYMBOL_TOKENS
        };
        #undef T

        #define T(k,v) make_pair(TokenType::k, #k),
        static map<TokenType, string> Type2Name {
            META_TOKENS
            KEYWORD_TOKENS
            SYMBOL_TOKENS
        };
        #undef T

        #define T(k,v) make_pair(TokenType::k, v),
        static map<TokenType, string> Type2Value {
            META_TOKENS
            KEYWORD_TOKENS
            SYMBOL_TOKENS
        };
        #undef T

        #define T(k,v) v,
        static vector<string> TokenValues {
            META_TOKENS
            KEYWORD_TOKENS
            SYMBOL_TOKENS
        };
        #undef T

        typedef struct Token {
            TokenType Type;
            unsigned int Index;
            unsigned int Count;
            const vector<char> *pChars;

            unsigned int 
            Line() {
                regex const pattern("\r?\n");
                string head = string(pChars->begin(), pChars->begin() + Index);
                ptrdiff_t const count(distance(
                    sregex_iterator(head.begin(), head.end(), pattern),
                    sregex_iterator()));
                return count + 1;
            }

            unsigned int
            Column() {
                unsigned int count = Index;
                for (count; count > 0; --count) {
                    char c = (*pChars)[count];
                    if ('\n' == c || '\r' == c) {
                        break;
                    }
                }
                return Index - count;
            }

            const string
            Value() {
                return string(pChars->begin() + Index, pChars->begin() + Index + Count);
            }

            const string
            Pretty() {
                auto value = Value();
                auto tokenvalue = Type2Value[Type];
                switch (Type) {
                case TokenType::WhiteSpace:
                case TokenType::EndOfFile:
                case TokenType::EndOfLine:
                case TokenType::Indent:
                case TokenType::Dedent:
                    return tokenvalue;
                default:
                    return value != tokenvalue ? tokenvalue + ":" + value : value;
                }
            }

            operator bool() {
                return Type != TokenType::EndOfFile;
            }
            bool operator==(const Token &rhs) {
                return Type == rhs.Type && Index == rhs.Index && Count == rhs.Count && pChars == rhs.pChars;
            }
            bool operator!=(const Token &rhs) {
                return Type != rhs.Type || Index != rhs.Index || Count != rhs.Count || pChars != rhs.pChars;
            }

        } Token;
    }
}

#endif /*__SOTA_TOKEN__*/