#include "z2h/token.hpp"
#include "z2h/parser.hpp"

#include "ast.hpp"
#include "parser.h"
#include "bindpower.h"
#include "exceptions.h"

#include <map>
#include <vector>
#include <boost/regex.hpp>
#include <iostream>

namespace sota {

    // scanners
    z2h::Token * SotaParser::SkippingScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
       return RegexScanner(symbol, source, position);
    }

    z2h::Token * SotaParser::RegexScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        z2h::Token *token = nullptr;
        auto pattern = symbol->pattern;
        boost::smatch matches;
        boost::regex re("^(" + pattern + ")");
        if (boost::regex_search(source, matches, re)) {
            auto match = matches[0];
            token = new z2h::Token(symbol, match, position, match.length(), false);
        }
        return token;
    }

    z2h::Token * SotaParser::LiteralScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        auto pattern = symbol->pattern;
        auto patternSize = pattern.size();
        if (source.size() >= patternSize && source.compare(0, patternSize, pattern) == 0) {
            return new z2h::Token(symbol, pattern, position, patternSize, false);
        }
        return nullptr;
    }

    z2h::Token * SotaParser::EosScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        if (!nesting.size()) {
            return RegexScanner(symbol, source, position);
        }
        return nullptr;
    }

    z2h::Token * SotaParser::EoeScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        if (nesting.size()) {
        }
        return nullptr;
    }


    z2h::Token * SotaParser::DentingScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        return nullptr;
    }

    // std parsing functions
    z2h::Ast * SotaParser::NotImplementedStd() {
        throw SotaNotImplemented(__FILE__, __LINE__, "std: NotImplemented; this shouldn't be called!");
    }

    // nud parsing functions
    z2h::Ast * SotaParser::NotImplementedNud(z2h::Token *token) {
        std::cout << "ni nud; token=" << *token << std::endl;
        throw SotaNotImplemented(__FILE__, __LINE__, "nud: NotImplemented; this shouldn't be called!");
    }
    z2h::Ast * SotaParser::EndOfFileNud(z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::NewlineNud(z2h::Token *token) {
        std::cout << "newline" << std::endl;
        return new NewlineAst(token);
    }
    z2h::Ast * SotaParser::NumberNud(z2h::Token *token) {
        return new NumberAst(token);
    }
    z2h::Ast * SotaParser::IdentifierNud(z2h::Token *token) {
        return new IdentifierAst(token);
    }
    z2h::Ast * SotaParser::PrefixNud(z2h::Token *token) {
        z2h::Ast *right = Expression(BindPower::Unary);
        return new PrefixAst(token, right);
    }
    z2h::Ast * SotaParser::IfThenElifElseNud(z2h::Token *token) {
        return nullptr;
    }

    // led parsing functions
    z2h::Ast * SotaParser::NotImplementedLed(z2h::Ast *left, z2h::Token *token) {
        std::cout << "ni led; token=" << *token  << " left=" << *left << std::endl;
        throw SotaNotImplemented(__FILE__, __LINE__, "led: NotImplemented; this shouldn't be called!");
    }
    z2h::Ast * SotaParser::EndOfFileLed(z2h::Ast *left, z2h::Token *token) {
        return left;
    }
    z2h::Ast * SotaParser::ComparisonLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::InfixLed(z2h::Ast *left, z2h::Token *token) {
        z2h::Ast *right = Expression(token->symbol->lbp);
        return new InfixAst(token, left, right);
    }
    z2h::Ast * SotaParser::PostfixLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::AssignLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::RegexLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::TernaryLed(z2h::Ast *left, z2h::Token *token) {
        auto action = Expression();
        auto pair = ConditionalAst::Pair(left, action);
        auto expected = symbolmap[SotaParser::SymbolType::Colon];
        Consume(expected, "colon : expected");
        auto defaultAction = Expression();
        return new ConditionalAst(token, {pair}, defaultAction);
    }
    z2h::Ast * SotaParser::IfThenElseLed(z2h::Ast *left, z2h::Token *token) {
        auto predicate = Expression();
        auto pair = ConditionalAst::Pair(predicate, left);
        auto expected = symbolmap[SotaParser::SymbolType::Else];
        Consume(expected, "else expected");
        auto defaultAction = Expression();
        return new ConditionalAst(token, {pair}, defaultAction);
    }

    SotaParser::SotaParser() {

        #define T(k,p,b,s,t,n,l) { SymbolType::k, new z2h::Symbol(SymbolType::k, b, p, SCAN(s), STD(t), NUD(n), LED(l) ) },
        symbolmap = {
            SYMBOLS
        };
        #undef T
    }

    std::exception SotaParser::Exception(const char *file, size_t line, const std::string &message) {
        return SotaException(file, line, message);
    }

    std::vector<z2h::Symbol *> SotaParser::Symbols() {
        std::vector<z2h::Symbol *> symbols;
        for (auto kvp : symbolmap) {
            symbols.push_back(kvp.second);
        }
        return symbols;
    }
}
