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
    long SkippingScanner(Symbol *symbol, const std::string &source, size_t index) {
       return RegexScanner(symbol, source, index) * -1; //returns a neg number if matched
    }
    long RegexScanner(Symbol *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        boost::smatch matches;
        boost::regex re("^" + pattern);
        if (boost::regex_search(source, matches, re)) {
            auto match = matches[0];
            return match.length();
        }
        return 0;
    }

    long LiteralScanner(Symbol *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        auto patternSize = pattern.size();
        if (source.size() >= patternSize && source.compare(0, patternSize, pattern) == 0) {
            return  patternSize;
        }
        return 0;
    }

    // nud parsing functions
    Ast * NotImplementedNud(Parser *parser, Token *token) {
        std::cout << "ni nud; token=" << *token << std::endl;
        throw SotaNotImplemented("nud: NotImplemented; this shouldn't be called!");
    }
    Ast * EndOfFileNud(Parser *parser, Token *token) {
        return nullptr;
    }
    Ast * WhiteSpaceNud(Parser *parser, Token *token) {
        return nullptr;
    }
    Ast * NewlineNud(Parser *parser, Token *token) {
        std::cout << "newline" << std::endl;
        return new NewlineAst(token->Value());
    }
    Ast * NumberNud(Parser *parser, Token *token) {
        return new NumberAst(token->Value());
    }
    Ast * IdentifierNud(Parser *parser, Token *token) {
        return new IdentifierAst(token->Value());
    }
    Ast * PrefixOperatorNud(Parser *parser, Token *token) {
        return nullptr;
    }

    // led parsing functions
    Ast * NotImplementedLed(Parser *parser, Ast *left, Token *token) {
        std::cout << "ni led; token=" << *token  << " left=" << left->Print() << std::endl;
        throw SotaNotImplemented("led: NotImplemented; this shouldn't be called!");
    }
    Ast * InfixOperatorLed(Parser *parser, Ast *left, Token *token) {
        Ast *right = parser->Parse(token->symbol.lbp);
        return new InfixOperatorAst(*token, left, right);
    }
    Ast * PostfixOperatorLed(Parser *parser, Ast *left, Token *token) {
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
