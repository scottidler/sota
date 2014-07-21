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

    long SotaParser::EosScanner(SotaSymbol *symbol, const std::string &source, size_t index) {
        if (!this->nesting.size()) {
            return RegexScanner(symbol, source, index);
        }
        return 0;
    }

    long SotaParser::EoeScanner(SotaSymbol *symbol, const std::string &source, size_t index) {
        if (this->nesting.size()) {
        }
        return 0;
    }


    long SotaParser::DentingScanner(SotaSymbol *symbol, const std::string &source, size_t index) {
        return 0;
    }

    // std parsing functions
    Ast * SotaParser::NotImplementedStd() {
        throw SotaNotImplemented("std: NotImplemented; this shouldn't be called!");
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

        //example calls, proving it works
        auto std1 = BindStd(&SotaParser::Nullptr);
        auto std2 = BindStd(&SotaParser::NotImplementedStd);
        auto nud1 = BindNud(&SotaParser::Nullptr);
        auto nud2 = BindNud(&SotaParser::NumberNud);
        auto led1 = BindLed(&SotaParser::Nullptr);
        auto led2 = BindLed(&SotaParser::PostfixLed);
        auto scan1 = BindScan(&SotaParser::Nullptr);
        auto scan2 = BindScan(&SotaParser::LiteralScanner); 

        #define T(k,p,b,s,t,n,l) { SymbolType::k, new SotaSymbol(SymbolType::k, b, p, SCAN(s), STD(t), NUD(n), LED(l) ) }, \

        symbolmap = {
            SYMBOLS
        };
        #undef T
    }

    std::vector<SotaSymbol *> SotaParser::Symbols() {
        std::vector<SotaSymbol *> symbols;
        for (auto kvp : symbolmap) {
            symbols.push_back(kvp.second);
        }
        return symbols;
    }
}
