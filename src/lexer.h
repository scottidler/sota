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
    class SotaLexer : public SotaStream<char> {

        unsigned int _stride;
        std::stack<unsigned int> _indents;
        std::deque<Token> _cache;

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
        SotaLexer(std::vector<char> chars);

        unsigned int Line(const Token &token);
        unsigned int Column(const Token &token);
        std::string Value(const Token &token);
        std::string Pretty(const Token &token);

        Token Scan();
        std::vector<Token> Tokenize();
    };
}

#endif /* __SOTA_LEXER__ */
