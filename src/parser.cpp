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
    SotaToken * SotaParser::SkippingScanner(SotaSymbol *symbol, const std::string &source, size_t position) {
       return RegexScanner(symbol, source, position);
    }

    SotaToken * SotaParser::RegexScanner(SotaSymbol *symbol, const std::string &source, size_t position) {
        SotaToken *token = nullptr;
        auto pattern = symbol->pattern;
        boost::smatch matches;
        boost::regex re("^(" + pattern + ")");
        if (boost::regex_search(source, matches, re)) {
            auto match = matches[0];
            token = new SotaToken(symbol, match, position, match.length(), false);
        }
        return token;
    }

    SotaToken * SotaParser::LiteralScanner(SotaSymbol *symbol, const std::string &source, size_t position) {
        auto pattern = symbol->pattern;
        auto patternSize = pattern.size();
        if (source.size() >= patternSize && source.compare(0, patternSize, pattern) == 0) {
            return new SotaToken(symbol, pattern, position, patternSize, false);
        }
        return nullptr;
    }

    SotaToken * SotaParser::EosScanner(SotaSymbol *symbol, const std::string &source, size_t position) {
        if (!nesting.size()) {
            return RegexScanner(symbol, source, position);
        }
        return nullptr;
    }

    SotaToken * SotaParser::EoeScanner(SotaSymbol *symbol, const std::string &source, size_t position) {
        if (nesting.size()) {
        }
        return nullptr;
    }


    SotaToken * SotaParser::DentingScanner(SotaSymbol *symbol, const std::string &source, size_t position) {
        return nullptr;
    }

    // std parsing functions
    Ast * SotaParser::NotImplementedStd() {
        throw SotaNotImplemented(__FILE__, __LINE__, "std: NotImplemented; this shouldn't be called!");
    }

    // nud parsing functions
    Ast * SotaParser::NotImplementedNud(SotaToken *token) {
        std::cout << "ni nud; token=" << *token << std::endl;
        throw SotaNotImplemented(__FILE__, __LINE__, "nud: NotImplemented; this shouldn't be called!");
    }
    Ast * SotaParser::EndOfFileNud(SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::NewlineNud(SotaToken *token) {
        std::cout << "newline" << std::endl;
        return new NewlineAst(token->value);
    }
    Ast * SotaParser::NumberNud(SotaToken *token) {
        return new NumberAst(token->value);
    }
    Ast * SotaParser::IdentifierNud(SotaToken *token) {
        return new IdentifierAst(token->value);
    }
    Ast * SotaParser::PrefixNud(SotaToken *token) {
        Ast *right = Expression(BindPower::Unary);
        return new PrefixAst(token, right);
    }
    Ast * SotaParser::IfThenElifElseNud(SotaToken *token) {
        return nullptr;
    }

    // led parsing functions
    Ast * SotaParser::NotImplementedLed(Ast *left, SotaToken *token) {
        std::cout << "ni led; token=" << *token  << " left=" << left->Print() << std::endl;
        throw SotaNotImplemented(__FILE__, __LINE__, "led: NotImplemented; this shouldn't be called!");
    }
    Ast * SotaParser::EndOfFileLed(Ast *left, SotaToken *token) {
        return left;
    }
    Ast * SotaParser::ComparisonLed(Ast *left, SotaToken *token) {
        return nullptr;
    }
    Ast * SotaParser::InfixLed(Ast *left, SotaToken *token) {
        Ast *right = Expression(token->symbol->lbp);
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
        auto action = Expression();
        auto pair = ConditionalAst::Pair(left, action);
        auto expected = symbolmap[SotaParser::SymbolType::Colon];
        Consume(expected, "colon : expected");
        auto defaultAction = Expression();
        return new ConditionalAst({pair}, defaultAction);
    }
    Ast * SotaParser::IfThenElseLed(Ast *left, SotaToken *token) {
        auto predicate = Expression();
        auto pair = ConditionalAst::Pair(predicate, left);
        auto expected = symbolmap[SotaParser::SymbolType::Else];
        Consume(expected, "else expected");
        auto defaultAction = Expression();
        return new ConditionalAst({pair}, defaultAction);
    }

    SotaParser::SotaParser() {

        #define T(k,p,b,s,t,n,l) { SymbolType::k, new SotaSymbol(SymbolType::k, b, p, SCAN(s), STD(t), NUD(n), LED(l) ) },
        symbolmap = {
            SYMBOLS
        };
        #undef T
    }

    std::exception SotaParser::Exception(const char *file, size_t line, const std::string &message) {
        return SotaException(file, line, message);
    }

    std::vector<SotaSymbol *> SotaParser::Symbols() {
        std::vector<SotaSymbol *> symbols;
        for (auto kvp : symbolmap) {
            symbols.push_back(kvp.second);
        }
        return symbols;
    }
}
