#include "grammar.h"

#include "ast.h"
#include "token.h"
#include "parser.h"
#include "grammar.h"
#include "exceptions.h"

#include <map>
#include <regex>
#include <iostream>

namespace sota {

    // scanners
    size_t RegexScanner(Symbol *symbol, const std::string &source, size_t index) {
        std::cout << "RegexScanner; Pattern: " << symbol->Pattern << std::endl;
        std::regex re(symbol->Pattern);
        std::smatch match;
        std::cout << "before" << std::endl;
        if (std::regex_match(source, match, re))
            std::cout << "after" << std::endl;
            std::cout << match.str() << std::endl;
            return match.size();
        return index;
    }

    size_t LiteralScanner(Symbol *symbol, const std::string &source, size_t index) {
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
    std::map<size_t, Symbol *> Type2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,n,l,b) std::make_pair(#k, Type2Symbol[SymbolType::k]),
    std::map<std::string, Symbol *> Name2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,n,l,b) std::make_pair(SymbolType::k, #k),
    std::map<size_t, std::string> Type2Name {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,n,l,b) std::make_pair(#k, SymbolType::k),
    std::map<std::string, size_t> Name2Type {
        SYMBOLS
    };
    #undef T

}
