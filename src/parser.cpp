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
    long SotaParser::SkippingScanner(SotaSymbol *symbol, const std::string &source, size_t index) {
       return RegexScanner(symbol, source, index) * -1; //returns a neg number if matched
    }
    long SotaParser::RegexScanner(SotaSymbol *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        boost::smatch matches;
        boost::regex re("^" + pattern);
        if (boost::regex_search(source, matches, re)) {
            auto match = matches[0];
            return match.length();
        }
        return 0;
    }

    long SotaParser::LiteralScanner(SotaSymbol *symbol, const std::string &source, size_t index) {
        auto pattern = symbol->pattern;
        auto patternSize = pattern.size();
        if (source.size() >= patternSize && source.compare(0, patternSize, pattern) == 0) {
            return  patternSize;
        }
        return 0;
    }

    // nud parsing functions
    Ast * SotaParser::NotImplementedNud(SotaToken *token) {
        std::cout << "ni nud; token=" << *token << std::endl;
        throw SotaNotImplemented("nud: NotImplemented; this shouldn't be called!");
    }
    Ast * SotaParser::EndOfFileNud(SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::WhiteSpaceNud(SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::NewlineNud(SotaToken *token) {
        std::cout << "newline" << std::endl;
        return new NewlineAst(token->Value());
    }
    Ast * SotaParser::NumberNud(SotaToken *token) {
        return new NumberAst(token->Value());
    }
    Ast * SotaParser::IdentifierNud(SotaToken *token) {
        return new IdentifierAst(token->Value());
    }
    Ast * SotaParser::PrefixNud(SotaToken *token) {
        Ast *right = this->Expression(BindPower::Unary);
        return new PrefixAst(token, right);
    }
    Ast * SotaParser::IfThenElifElseNud(SotaToken *token) {
        return nullptr;
    }

    // led parsing functions
    Ast * SotaParser::NotImplementedLed(Ast *left, SotaToken *token) {
        std::cout << "ni led; token=" << *token  << " left=" << left->Print() << std::endl;
        throw SotaNotImplemented("led: NotImplemented; this shouldn't be called!");
    }
    Ast * SotaParser::ComparisonLed(Ast *left, SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::InfixLed(Ast *left, SotaToken *token) {
        Ast *right = this->Expression(token->symbol->lbp);
        return new InfixAst(token, left, right);
    }
    Ast * SotaParser::PostfixLed(Ast *left, SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::AssignLed(Ast *left, SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::RegexLed(Ast *left, SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::TernaryLed(Ast *left, SotaToken *token) {
        auto action = this->Expression();
        auto pair = ConditionalAst::Pair(left, action);
        this->Consume(SotaParser::SymbolType::Colon, "colon : expected");
        auto defaultAction = this->Expression();
        return new ConditionalAst({pair}, defaultAction);
    }
    Ast * SotaParser::IfThenElseLed(Ast *left, SotaToken *token) {
        auto predicate = this->Expression();
        auto pair = ConditionalAst::Pair(predicate, left);
        this->Consume(SotaParser::SymbolType::Else, "else expected");
        auto defaultAction = this->Expression();
        return new ConditionalAst({pair}, defaultAction);
    }

    SotaParser::SotaParser() {
        std::map<size_t, SotaSymbol *> Test {
            std::make_pair(SotaParser::SymbolType::Add, new SotaSymbol(SotaParser::SymbolType::Add, BindPower::Sum, "+", std::bind(&SotaParser::LiteralScanner, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), nullptr, nullptr, nullptr) ),
        };
    }
/*
    #define T(k,p,s,t,n,l,b) std::make_pair(SotaParser::SymbolType::k, new SotaSymbol(SotaParser::SymbolType::k,p,&SotaParser::s,&SotaParser::t,&SotaParser::n,&SotaParser::l,b) ),
    std::map<size_t, SotaSymbol *> Type2Symbol {
        SYMBOLS
    };
    #undef T
*/
/*
    #define T(k,p,s,t,n,l,b) std::make_pair(#k, Type2Symbol[SotaParser::SymbolType::k]),
    std::map<std::string, SotaSymbol *> Name2Symbol {
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

    std::vector<SotaSymbol *> SotaParser::Symbols() {
        std::vector<SotaSymbol *> symbols;
        return symbols;
    }
}
