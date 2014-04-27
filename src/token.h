#ifndef __SOTA_TOKENS__
#define __SOTA_TOKENS__ = 1

#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include "utils.h"

using namespace std;

namespace sota {
    namespace lexer {
        #define TOKEN_TABLE                     \
        T(EndOfFile         ,   "<EOF>")        \
        T(BegOfFile         ,   "<BOF>")        \
        T(BegOfZone         ,   "<BOS>")        \
        T(EndOfZone         ,   "<EOS>")        \
        T(BegOfLine         ,   "<BOL>")        \
        T(EndOfLine         ,   "<EOL>")        \
        T(Indent            ,   "<INDENT>")     \
        T(Dedent            ,   "<DEDENT>")     \
        T(WhiteSpace        ,   "<WS>")         \
        T(Comment           ,   "<COMMENT>")    \
        T(Id                ,   "<ID>")         \
        T(Num               ,   "<NUM>")        \
                                                \
        T(As                ,   "as")           \
        T(In                ,   "in")           \
        T(Or                ,   "or")           \
        T(Not               ,   "not")          \
        T(If                ,   "if")           \
        T(Elif              ,   "elif")         \
        T(Else              ,   "else")         \
        T(With              ,   "with")         \
        T(While             ,   "while")        \
        T(Try               ,   "try")          \
        T(Catch             ,   "catch")        \
        T(Raise             ,   "raise")        \
        T(Match             ,   "match")        \
        T(Foreach           ,   "foreach")      \
        T(Parallel          ,   "parallel")     \
        T(Print             ,   "print")        \
        T(Debug             ,   "debug")        \
        T(Trace             ,   "trace")        \
        T(Async             ,   "async")        \
        T(Await             ,   "await")        \
        T(Yield             ,   "yield")        \
        T(Break             ,   "break")        \
        T(Continue          ,   "continue")     \
        T(From              ,   "from")         \
        T(Import            ,   "import")       \
        T(Insert            ,   "insert")       \
        T(This              ,   "this")         \
        T(Base              ,   "base")         \
        T(Null              ,   "null")         \
        T(True              ,   "true")         \
        T(False             ,   "false")        \
                                                \
        T(LeftParen         ,   "(")            \
        T(RightParen        ,   ")")            \
        T(LeftBracket       ,   "[")            \
        T(RightBracket      ,   "]")            \
        T(LeftBrace         ,   "{")            \
        T(RightBrace        ,   "}")            \
        T(Backtic           ,   "`")            \
        T(SingleQuote       ,   "\'")           \
        T(DoubleQuote       ,   "\"")           \
        T(Colon             ,   ":")            \
        T(SemiColon         ,   ";")            \
        T(Comma             ,   ",")            \
                                                \
        T(Tilde             ,   "~")            \
        T(Dot               ,   ".")            \
        T(Updir             ,   "../")          \
                                                \
        T(RegexMatch        ,   "m/")           \
        T(RegexSubstitute   ,   "s/")           \
                                                \
        T(Add               ,   "+")            \
        T(Sub               ,   "-")            \
        T(Mul               ,   "*")            \
        T(Div               ,   "/")            \
        T(BitOr             ,   "|")            \
        T(BitAnd            ,   "&")            \
        T(GreaterThan       ,   ">")            \
        T(LessThan          ,   "<")            \
        T(Assign            ,   "=")            \
        T(AddAssign         ,   "+=")           \
        T(AssignAdd         ,   "=+")           \
        T(SubAssign         ,   "-=")           \
        T(AssignSub         ,   "=-")           \
        T(MulAssign         ,   "*=")           \
        T(AssignMul         ,   "=*")           \
        T(DivAssign         ,   "/=")           \
        T(AssignDiv         ,   "=/")           \
        T(LogOr             ,   "||")           \
        T(LogAnd            ,   "&&")           \
        T(LogEq             ,   "==")           \
        T(GreaterThanEquals ,   ">=")           \
        T(LessThanEquals    ,   "<=")           \
        T(RightBitShift     ,   ">>")           \
        T(LeftBitShift      ,   "<<")           \
        T(RangeInclusive    ,   "..")           \
        T(RangeExclusive    ,   "...")          \
                                                \
        T(RightArrow        ,   "->")           \
        T(LeftArrow         ,   "<-")

        #define T(k,v) k,
        enum TokenType {
            TOKEN_TABLE
        };
        #undef T

        #define T(k,v) make_pair(v, TokenType::k),
        static map<string, TokenType> Value2Type {
            TOKEN_TABLE 
        };
        #undef T

        #define T(k,v) make_pair(TokenType::k, #k),
        static map<TokenType, string> Type2Name {
            TOKEN_TABLE
        };
        #undef T

        #define T(k,v) make_pair(TokenType::k, v),
        static map<TokenType, string> Type2Value {
            TOKEN_TABLE
        };
        #undef T

        #define T(k,v) v,
        static vector<string> TokenValues {
            TOKEN_TABLE
        };
        #undef T

        typedef struct Token_t {
            TokenType type;
            string value;
            Token_t() : type(TokenType::EndOfFile), value("") {}
            Token_t(TokenType token, string value = "") : type(token), value(value) {}

            operator bool() { 
                return type != TokenType::EndOfFile;
            }

            friend ostream& operator<< (ostream &o, Token_t token) {
                o << "[ " << token.type << (token.type == TokenType::Id || token.type == TokenType::Num ? "(" + Type2Name[token.type] + ")" : "") << " " << token.value << " ]";
                return o;
            }

        } Token;
    }
}

#endif /*__SOTA_TOKENS__*/