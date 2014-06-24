#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <queue>
#include <stack>
#include <string>

#include "ast.h"
#include "token.h"

namespace sota {

    class Ast;
    class Parser {

        size_t              _index;
        size_t              _stride;
        std::stack<size_t>  _indents;
        std::stack<Token>   _nesting;
        std::deque<Token>   _tokens;
        std::string         _source;

        std::string Load(const std::string &filename);
        Token Take(Token token);
        Token Emit();
        Token Scan();
        Token LookAhead(int distance);

    public:

        Parser();
        Parser(const std::string &source);

        Ast * ParseFile(const std::string &filename);
        Ast * Parse(const std::string &source);
        Ast * Parse(size_t lbp = 0);

        Token Consume();
        Token Consume(const SymbolType &expected, const std::string &message);

        size_t Line();
        size_t Column();
    };
}

#endif /*__SOTA_PARSER__*/
