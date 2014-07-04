#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "grammar.h"
#include "exceptions.h"

#include "symbol.h"

namespace sota {

    class Ast;
    class Token;
    class Parser;

    typedef struct Token {

        Symbol              symbol;
        std::string         source;
        size_t              index;
        size_t              length;
        bool                skip;

        Token();
        Token(const Symbol &symbol, const std::string &source, size_t index, size_t length, bool skip);

        std::string         Value() const;

        Ast *               Nud(Parser *parser, Token *token);
        Ast *               Led(Parser *parser, Ast *ast, Token *token);

        operator bool();
        friend std::ostream & operator<<(std::ostream &out, const Token &token);

    } Token;
}

#endif /*__SOTA_TOKEN__*/
