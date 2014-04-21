#ifndef __SOTA_LEXER__
#define __SOTA_LEXER__ = 1

#include <string>
#include <vector>
#include "token.h"

namespace sota {
    namespace lexer {
        class SotaLexer {
            vector<Token> _tokens;
            string _code;
            int _index;
            int _line;
            int _col;

            char prevchar(int lookback = 1);
            char peekchar(int lookahead = 1);
            char nextchar();
            bool ischar(char c, bool advance = false);
            bool inrange(char c1, char c2, bool advance = false);
            bool isempty(bool advance = false);
        public:
            SotaLexer(string filename);
            ~SotaLexer();
            void Load(string filename);
            Token Prev(int lookback = 1);
            Token Peek(int lookahead = 1);
            Token Next();
        };
    }
}
#endif /*__SOTA_LEXER__*/