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
    using namespace utils;
    using namespace stream;
    namespace lexer {
        class SotaLexer {
            string _filename;

            vector<char> _chars;
            SotaStream<char> _charstream;

            vector<Token> _tokens;
            SotaStream<Token> _tokenstream;

            unsigned int _stride;
            stack<unsigned int> _indents;

            inline Token eol();
            inline Token dent();
            inline Token ws();
            inline Token comment();
            inline Token lit();
            inline Token sym();
            inline Token id_num_kw();
            inline Token eof();

        public:
            ~SotaLexer();
            SotaLexer(string filename);
            void Load(string filename);
            Token Scan();
            vector<Token> Tokenize();
        };
    }
}

#endif /* __SOTA_LEXER__ */