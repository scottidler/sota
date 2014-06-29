#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "exceptions.h"
#include "grammar.h"

namespace sota {

    class Ast;
    class Token;
    class Parser;
    class Symbol;

    enum BindPower: size_t {
        None = 0,
        Sum = 10,
        Product = 20,
        Negation = 30,
    };

    typedef std::function<long(Symbol *, const std::string &, size_t)> ScanFunc;
    typedef std::function<Ast *(Parser *, Token *)> NudFunc;
    typedef std::function<Ast *(Parser *, Ast *, Token *)> LedFunc;

    class Ast;
    class Token;
    class Parser;

    typedef struct Symbol {

        SymbolType      type;
        std::string     pattern;
        ScanFunc        Scan;
        NudFunc         Nud;
        LedFunc         Led;
        size_t          lbp;

        Symbol();
        Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp);

        operator bool();
        bool operator==(const Symbol &rhs);
        bool operator!=(const Symbol &rhs);

        friend std::ostream & operator<<(std::ostream &out, const Symbol &symbol);

    } Symbol;

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
