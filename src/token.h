#ifndef __SOTA_TOKEN__
#define __SOTA_TOKEN__ = 1

#include <map>
#include <string>
#include <functional>

//#include "ast.h"
//#include "parser.h"
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
    };

    class Token {

        Symbol              *_symbol;

        public:
        const std::string   &Source;
        size_t              Index;
        size_t              Length;

        Token();
        Token(Symbol *symbol, const std::string &source, size_t index, size_t length);

        SymbolType Type() const;
        std::string Pattern() const;
        size_t LBP() const;
        std::string Value() const;
        size_t Scan(size_t index);
        Ast * Parse(Parser *parser, Ast *ast, Token *token);
        operator bool();
        friend std::ostream & operator<<(std::ostream &out, const Token &token);
    };

    size_t EndOfFileScanner(const std::string &source, size_t index);
    size_t RegexScanner(const std::string &source, size_t index);
    size_t LiteralScanner(const std::string &source, size_t index);
    Ast * NullParser(Parser *parser, Ast *ast, Token *token);
    Ast * InfixParser(Parser *parser, Ast *ast, Token *token);
}

#endif /*__SOTA_TOKEN__*/
