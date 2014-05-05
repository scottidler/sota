#ifndef __SOTA_LEXER__
#define __SOTA_LEXER__ = 1

#include <stack>
#include <string>
#include <vector>
#include "token.h"
#include "stream.hpp"

namespace sota {
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
            inline Token raw();
            inline Token str();
            inline Token eof();
    
            //inline bool nl();
            ////inline bool lit();
            //inline bool ws();
            //inline bool raw();
            //inline bool str();

        public:
            ~SotaLexer();
            SotaLexer(string filename);
            void Load(string filename);
            vector<Token> Pass1();
            vector<Token> Pass2();
            vector<Token> Tokenize();
        };
    }
}

#endif /* __SOTA_LEXER__ */