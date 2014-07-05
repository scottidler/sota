#include "z2h/token.hpp"
#include "z2h/parser.hpp"

#include "ast.h"
#include "parser.h"
#include "bindpower.h"
#include "exceptions.h"

#include <map>
#include <vector>
#include <boost/regex.hpp>
#include <iostream>

namespace sota {

    // scanners
    long SotaParser::SkippingScanner(z2h::Symbol<Ast *> *symbol, const std::string &source, size_t index) {
       return RegexScanner(symbol, source, index) * -1; //returns a neg number if matched
    }
    long SotaParser::RegexScanner(z2h::Symbol<Ast *> *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        boost::smatch matches;
        boost::regex re("^" + pattern);
        if (boost::regex_search(source, matches, re)) {
            auto match = matches[0];
            return match.length();
        }
        return 0;
    }

    long SotaParser::LiteralScanner(z2h::Symbol<Ast *> *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        auto patternSize = pattern.size();
        if (source.size() >= patternSize && source.compare(0, patternSize, pattern) == 0) {
            return  patternSize;
        }
        return 0;
    }

    // nud parsing functions
    Ast * SotaParser::NotImplementedNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        std::cout << "ni nud; token=" << *token << std::endl;
        throw SotaNotImplemented("nud: NotImplemented; this shouldn't be called!");
    }
    Ast * SotaParser::EndOfFileNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        return nullptr;
    }
    Ast * SotaParser::WhiteSpaceNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        return nullptr;
    }
    Ast * SotaParser::NewlineNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        std::cout << "newline" << std::endl;
        return new NewlineAst(token->Value());
    }
    Ast * SotaParser::NumberNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        return new NumberAst(token->Value());
    }
    Ast * SotaParser::IdentifierNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        return new IdentifierAst(token->Value());
    }
    Ast * SotaParser::PrefixNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        Ast *right = parser->Expression(BindPower::Unary);
        return new PrefixAst(token, right);
    }
    Ast * SotaParser::IfThenElifElseNud(z2h::Parser<Ast *> *parser, z2h::Token<Ast *> *token) {
        return nullptr;
    }

    // led parsing functions
    Ast * SotaParser::NotImplementedLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        std::cout << "ni led; token=" << *token  << " left=" << left->Print() << std::endl;
        throw SotaNotImplemented("led: NotImplemented; this shouldn't be called!");
    }
    Ast * SotaParser::ComparisonLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        return nullptr;
    }
    Ast * SotaParser::InfixLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        Ast *right = parser->Expression(token->symbol->lbp);
        return new InfixAst(token, left, right);
    }
    Ast * SotaParser::PostfixLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        return nullptr;
    }
    Ast * SotaParser::AssignLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        return nullptr;
    }
    Ast * SotaParser::RegexLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        return nullptr;
    }
    Ast * SotaParser::TernaryLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        auto action = parser->Expression();
        auto pair = ConditionalAst::Pair(left, action);
        parser->Consume(SotaParser::SymbolType::Colon, "colon : expected");
        auto defaultAction = parser->Expression();
        return new ConditionalAst({pair}, defaultAction);
    }
    Ast * SotaParser::IfThenElseLed(z2h::Parser<Ast *> *parser, Ast *left, z2h::Token<Ast *> *token) {
        auto predicate = parser->Expression();
        auto pair = ConditionalAst::Pair(predicate, left);
        parser->Consume(SotaParser::SymbolType::Else, "else expected");
        auto defaultAction = parser->Expression();
        return new ConditionalAst({pair}, defaultAction);
    }

    SotaParser::SotaParser() {
        std::map<size_t, z2h::Symbol<Ast *> *> Test {
            std::make_pair(SotaParser::SymbolType::Add, new z2h::Symbol<Ast *>(SotaParser::SymbolType::Add, BindPower::Sum, "+", std::bind(&SotaParser::LiteralScanner, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), nullptr, nullptr, nullptr) ),
        };
    }
/*
    #define T(k,p,s,t,n,l,b) std::make_pair(SotaParser::SymbolType::k, new z2h::Symbol<Ast *>(SotaParser::SymbolType::k,p,&SotaParser::s,&SotaParser::t,&SotaParser::n,&SotaParser::l,b) ),
    std::map<size_t, z2h::Symbol<Ast *> *> Type2Symbol {
        SYMBOLS
    };
    #undef T
*/
/*
    #define T(k,p,s,t,n,l,b) std::make_pair(#k, Type2Symbol[SotaParser::SymbolType::k]),
    std::map<std::string, z2h::Symbol<Ast *> *> Name2Symbol {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,t,n,l,b) std::make_pair(SotaParser::SymbolType::k, #k),
    std::map<size_t, std::string> Type2Name {
        SYMBOLS
    };
    #undef T

    #define T(k,p,s,t,n,l,b) std::make_pair(#k, SotaParser::SymbolType::k),
    std::map<std::string, size_t> Name2Type {
        SYMBOLS
    };
    #undef T
*/

    std::vector<z2h::Symbol<Ast *> *> SotaParser::Symbols() {
        std::vector<z2h::Symbol<Ast *> *> symbols;
        return symbols;
    }
}
