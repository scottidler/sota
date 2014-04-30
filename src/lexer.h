#ifndef __SOTA_LEXER__
#define __SOTA_LEXER__ = 1

#include <stack>
#include "token.h"
#include "stream.hpp"


namespace sota {
    using namespace stream;

    namespace lexer {

        inline bool isoneof(char c, string values) {
            for (auto value : values) {
                if (value == c)
                    return true;
            }
            return false;
        }

        inline bool startswith(string s, vector<string> values) {
            for (auto value : values) {
                if (value.find(s) == 0)
                    return true;
            }
            return false;
        }

        class SotaLexer : public SotaStream<char> {
            stack<unsigned int> _idents;
            stack<Token> _tokens;
            Token pop();
            Token dots(char &c);
            Token regex(char &c);
            Token comment(char &c);
            Token newline(char &c);
            Token whitespace(char &c);
            Token identifier_numeral_or_symbol(char &c);
        public:
            unsigned int Line;
            unsigned int Col;

            SotaLexer();
            SotaLexer(string filename);
            SotaLexer(vector<char> chars);

            const char&
            Next(unsigned int lookahead = 1);

            void 
            Load(string filename);

            void 
            Load(vector<char> chars);

            Token 
            Scan();
        };
    }
}

#endif /*__SOTA_LEXER__*/