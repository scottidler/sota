#include "grammar.h"

#include "ast.h"
#include "token.h"
#include "parser.h"
#include "symbol.h"
#include "grammar.h"
#include "exceptions.h"

#include <map>
#include <iostream>

namespace sota {

    // scanners
    size_t RegexScanner(const std::string &source, size_t index) {
        std::cout << "RegexScanner" << std::endl;
        return index;
    }

    size_t LiteralScanner(const std::string &source, size_t index) {
        std::cout << "LiteralScanner" << std::endl;
        return index;
    }

    // nud parsing functions
    Ast * NotImplementedNud(Parser *parser, Token *token) {
        throw SotaNotImplemented("nud: NotImplemented; this shouldn't be called!");
    }
    Ast * EndOfFileNud(Parser *parser, Token *token) {
        std::cout << "EndOfFileNud" << std::endl;
        return nullptr;
    }
    Ast * WhiteSpaceNud(Parser *parser, Token *token) {
        std::cout << "WhiteSpaceNud" << std::endl;
        return nullptr;
    }
    Ast * IdentifierNud(Parser *parser, Token *token) {
        std::cout << "IdentifierNud" << std::endl;
        return nullptr;
    }
    Ast * PrefixOperatorNud(Parser *parser, Token *token) {
        std::cout << "PrefixOperatorNud" << std::endl;
        return nullptr;
    }

    // led parsing functions
    Ast * NotImplementedLed(Parser *parser, Ast *left, Token *token) {
        throw SotaNotImplemented("led: NotImplemented; this shouldn't be called!");
    }
    Ast * InfixOperatorLed(Parser *parser, Ast *left, Token *token) {
        std::cout << "InfixOperatorLed" << std::endl;
        return left;
    }
    Ast * PostfixOperatorNud(Parser *parser, Ast *left, Token *token) {
        std::cout << "PostfixOperatorLed" << std::endl;
        return nullptr;
    }

    #define T(k,p,s,n,l,b) std::make_pair(SymbolType::k, new Symbol(SymbolType::k,p,s,n,l,b) ),
    std::map<SymbolType, Symbol *> Type2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,n,l,b) std::make_pair(#k, Type2Symbol[SymbolType::k]),
    std::map<std::string, Symbol *> Name2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,n,l,b) std::make_pair(SymbolType::k, #k),
    std::map<SymbolType, std::string> Type2Name {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,n,l,b) std::make_pair(#k, SymbolType::k),
    std::map<std::string, SymbolType> Name2Type {
        SYMBOLS
    };
    #undef T

}
