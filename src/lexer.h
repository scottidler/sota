#ifndef __SOTA_LEXER__
#define __SOTA_LEXER__ = 1

#include <queue>
#include <stack>
#include <string>
#include <vector>
#include "utils.h"
#include "token.h"
#include "stream.hpp"

namespace sota {
    class SotaLexer : SotaStream<char> {

        unsigned int _stride;
        stack<unsigned int> _indents;
        deque<Token> _cache;

        Token eol();
        Token dent();
        Token ws();
        Token comment();
        Token lit();
        Token sym();
        Token id_num_kw();
        Token eof();

    public:
        ~SotaLexer();
        SotaLexer(vector<char> chars);

        unsigned int Line(const Token &token);
        unsigned int Column(const Token &token);
        string Value(const Token &token);
        string Pretty(const Token &token);

        Token Scan();
        vector<Token> Tokenize();
    };
}

#endif /* __SOTA_LEXER__ */