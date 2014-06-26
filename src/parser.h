#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <queue>
#include <stack>
#include <string>

#include "ast.h"
#include "token.h"

namespace sota {

    typedef std::map<size_t, Symbol *> Types2Symbols;

    class Ast;

    class Parser {

        size_t              _stride;
        std::stack<size_t>  _indents;
        std::stack<Token>   _nesting;
        std::deque<Token>   _tokens;

    public:
        std::string Load(const std::string &filename);
        Token Take(Token token);
        Token Emit();
        Token Scan();
        Token LookAhead(size_t distance);

    public:

        Types2Symbols       Symbols;
        std::string         Source;
        size_t              Index;

        Parser(const Types2Symbols &symbols);

        Ast * ParseFile(const std::string &filename);
        Ast * Parse(std::string source);
        Ast * Parse(size_t lbp = 0);

        Token Consume();
        Token Consume(const size_t &expected, const std::string &message);

        size_t Line();
        size_t Column();
    };
}

#endif /*__SOTA_PARSER__*/
