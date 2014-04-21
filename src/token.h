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
        #define TOKEN_TABLE                 \
        T(META_UNDEF    ,   "")             \
        T(META_BOF      ,   "<BOF>")        \
        T(META_BOS      ,   "<BOS>")        \
        T(META_EOS      ,   "<EOS>")        \
        T(META_BOL      ,   "<BOL>")        \
        T(META_EOL      ,   "<EOL>")        \
        T(META_EOF      ,   "<EOF>")        \
        T(META_INDENT   ,   "<INDENT>")     \
        T(META_DEDENT   ,   "<DEDENT>")     \
        T(META_WS       ,   "<WS>")         \
        T(META_COMMENT  ,   "<COMMENT>")    \
        T(META_ID       ,   "<ID>")         \
        T(META_NUM      ,   "<NUM>")        \
                                            \
        T(KW_AS         ,   "as")           \
        T(KW_IN         ,   "in")           \
        T(KW_OR         ,   "or")           \
        T(KW_NOT        ,   "not")          \
        T(KW_IF         ,   "if")           \
        T(KW_ELIF       ,   "elif")         \
        T(KW_ELSE       ,   "else")         \
        T(KW_WITH       ,   "with")         \
        T(KW_WHILE      ,   "while")        \
        T(KW_TRY        ,   "try")          \
        T(KW_CATCH      ,   "catch")        \
        T(KW_RAISE      ,   "raise")        \
        T(KW_MATCH      ,   "match")        \
        T(KW_FOREACH    ,   "foreach")      \
        T(KW_PARALLEL   ,   "parallel")     \
        T(KW_PRINT      ,   "print")        \
        T(KW_DEBUG      ,   "debug")        \
        T(KW_TRACE      ,   "trace")        \
        T(KW_ASYNC      ,   "async")        \
        T(KW_AWAIT      ,   "await")        \
        T(KW_YIELD      ,   "yield")        \
        T(KW_BREAK      ,   "break")        \
        T(KW_CONTINUE   ,   "continue")     \
        T(KW_FROM       ,   "from")         \
        T(KW_IMPORT     ,   "import")       \
        T(KW_INSERT     ,   "insert")       \
        T(KW_THIS       ,   "this")         \
        T(KW_BASE       ,   "base")         \
        T(KW_NULL       ,   "null")         \
        T(KW_TRUE       ,   "true")         \
        T(KW_FALSE      ,   "false")        \
                                            \
        T(LP            ,   "(")            \
        T(RP            ,   ")")            \
        T(LBK           ,   "[")            \
        T(RBK           ,   "]")            \
        T(LBR           ,   "{")            \
        T(RBR           ,   "}")            \
        T(BACKTIC       ,   "`")            \
        T(SQUOTE        ,   "\'")           \
        T(DQUOTE        ,   "\"")           \
        T(COLON         ,   ":")            \
        T(SEMICOLON     ,   ";")            \
        T(COMMA         ,   ",")            \
                                            \
        T(TILDE         ,   "~")            \
        T(DOT           ,   ".")            \
        T(UPDIR         ,   "../")          \
                                            \
        T(REM           ,   "m/")           \
        T(RES           ,   "s/")           \
                                            \
        T(ADD           ,   "+")            \
        T(SUB           ,   "-")            \
        T(MUL           ,   "*")            \
        T(DIV           ,   "/")            \
        T(BITOR         ,   "|")            \
        T(BITAND        ,   "&")            \
        T(GT            ,   ">")            \
        T(LT            ,   "<")            \
        T(ASSIGN        ,   "=")            \
        T(ADDASSIGN     ,   "+=")           \
        T(ASSIGNADD     ,   "=+")           \
        T(SUBASSIGN     ,   "-=")           \
        T(ASSIGNSUB     ,   "=-")           \
        T(MULASSIGN     ,   "*=")           \
        T(ASSIGNMUL     ,   "=*")           \
        T(DIVASSIGN     ,   "/=")           \
        T(ASSIGNDIV     ,   "=/")           \
        T(LOGOR         ,   "||")           \
        T(LOGAND        ,   "&&")           \
        T(LOGEQ         ,   "==")           \
        T(GTE           ,   ">=")           \
        T(LTE           ,   "<=")           \
        T(RBITSHIFT     ,   ">>")           \
        T(LBITSHIFT     ,   "<<")           \
        T(RANGE         ,   "..")           \
                                            \
        T(RARROW        ,   "->")           \
        T(LARROW        ,   "<-")

        #define T(k,v) k,
        enum TokenType {
            TOKEN_TABLE
        };
        #undef T

        #define T(k,v) make_pair(v, TokenType::k),
        static map<string, TokenType> TokenTypes {
            TOKEN_TABLE
        };
        #undef T

        typedef struct Token_t {
            Token_t() : line(-1), col(-1), type(TokenType::META_UNDEF), value("") {}
            Token_t(int line, int col, TokenType token, string value = "") : line(line+1), col(col+1), type(token), value(value) {}
            int line;
            int col;
            TokenType type;
            string value;
        } Token;
    }
}

#endif /*__SOTA_TOKENS__*/