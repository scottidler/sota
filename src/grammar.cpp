#include "grammar.h"

#include "ast.h"
#include "token.h"
#include "parser.h"
#include "symbol.h"
#include "grammar.h"

#include <map>
#include <iostream>

namespace sota {

    size_t EndOfFileScanner(const std::string &source, size_t index) {
        std::cout << "EndOfFileScanner" << std::endl;
        return index;
    }

    size_t RegexScanner(const std::string &source, size_t index) {
        std::cout << "RegexScanner" << std::endl;
        return index;
    }

    size_t LiteralScanner(const std::string &source, size_t index) {
        std::cout << "LiteralScanner" << std::endl;
        return index;
    }

    Ast * NullParser(Parser *parser, Ast *ast, Token *token) {
        std::cout << "NullParser" << std::endl;
        return ast;
    }

    Ast * InfixParser(Parser *parser, Ast *ast, Token *token) {
        std::cout << "InfixOperator" << std::endl;
        return ast;
    }

    #define T(n,v,s,p,b) std::make_pair(SymbolType::n, new Symbol(SymbolType::n, v, s, p, b) ),
    std::map<SymbolType, Symbol *> Type2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(n,v,s,p,b) std::make_pair(#n, new Symbol(SymbolType::n, v, s, p, b) ),
    std::map<std::string, Symbol *> Name2Symbol {
        SYMBOLS
    };
    #undef T
}
