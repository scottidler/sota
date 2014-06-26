#include "grammar.h"

#include "ast.h"
#include "token.h"
#include "parser.h"
#include "grammar.h"
#include "exceptions.h"

#include <map>
#include <boost/regex.hpp>
#include <iostream>

namespace sota {

    // scanners
    size_t RegexScanner(Symbol *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        boost::smatch matches;
        boost::regex re("^" + pattern);
        if (boost::regex_search(source, matches, re)) {
            auto match = matches[0];
            //std::cout << "re pattern: " << pattern << " matched: " << match << std::endl;
            return index + match.length();
        }
        return index;
    }

    size_t LiteralScanner(Symbol *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        auto patternSize = pattern.size();
        if (source.size() >= patternSize && source.compare(0, patternSize, pattern) == 0) {
            //std::cout << "lit pattern: " << pattern << " matched: " << pattern << std::endl;
            return index + patternSize;
        }
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
    Ast * NumberNud(Parser *parser, Token *token) {
        std::cout << "NumberNud" << std::endl;
        return nullptr;
    }
    Ast * IdentifierNud(Parser *parser, Token *token) {
        std::cout << "IdentifierNud" << std::endl;
        Ast *ast = new IdentifierAst(token->value());
        return ast;
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
