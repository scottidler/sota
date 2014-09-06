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

    std::vector<z2h::Ast *> SotaParser::Expressions(z2h::Symbol *end) {
        std::vector<z2h::Ast *> expressions;
        auto eoe = symbolmap[SymbolType::EndOfExpression];
        auto la1 = this->LookAhead1();
        while (end != la1->symbol) {
            auto expression = this->Expression();
            expressions.push_back(expression);
            if (!this->Consume(eoe)) {
                break;
            }
            la1 = this->LookAhead1();
        }
        return expressions;
    }

    std::vector<z2h::Token *> SotaParser::TokenizeFile(const std::string &filename) {
        auto source = Open(filename);
        return Tokenize(source);
    }

    std::vector<z2h::Token *> SotaParser::Tokenize(std::string source) {
        this->index = 0;
        this->source = source;
        auto eof = EofSymbol();
        auto token = Consume();
        while (eof != token->symbol) {
            token = Consume();
        }
        return tokens;
    }

    z2h::Ast * SotaParser::ParseFile(const std::string &filename) {
        auto source = Open(filename);
        return Parse(source);
    }

    z2h::Ast * SotaParser::Parse(const std::string &source) {
        this->index = 0;
        this->source = source;
        auto statements = Statements();
        return new BlockAst(statements);
    }

/*
    std::vector<z2h::Ast *> SotaParser::Statements(z2h::Symbol *end) {
        std::vector<z2h::Ast *> statements;
        return statements;
    }
*/

    // scanners
    z2h::Token * SotaParser::SkippingScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        auto token = RegexScanner(symbol, source, position);
        if (token)
            token->skip = true;
        return token;
    }

    z2h::Token * SotaParser::RegexScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        z2h::Token *token = nullptr;
        boost::smatch matches;
        const boost::regex re("(" + symbol->pattern + ")(?:\n|.)*");
        if (boost::regex_match(source, matches, re)) {
            auto match = matches[1];
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
        return RegexScanner(symbol, source, position);
    }


    z2h::Token * SotaParser::DentingScanner(z2h::Symbol *symbol, const std::string &source, size_t position) {
        return nullptr;
    }

    // std parsing functions
    z2h::Ast * SotaParser::NullptrStd() {
        return nullptr;
    }

    // nud parsing functions
    z2h::Ast * SotaParser::NullptrNud(z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::EndOfFileNud(z2h::Token *token) {
        return nullptr;
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
    z2h::Ast * SotaParser::CommaNud(z2h::Token *token) {
        auto right = Expression(token->symbol->lbp);
        return new z2h::BinaryAst(token, new NullAst(), right);
    }
    z2h::Ast * SotaParser::ParensNud(z2h::Token *token) {
        auto rp = symbolmap[SymbolType::RightParen];
        auto ast = Expression();
        if (!this->Consume(rp)) {
            std::cout << "RightParen not consumed" << std::endl;
        }
        auto asts = ast->Vectorize();
        return new ParensAst(asts);
    }
    z2h::Ast * SotaParser::BracesNud(z2h::Token *token) {
        auto rb = symbolmap[SymbolType::RightBrace];
        auto ast = Expression();
        if (!this->Consume(rb)) {
            std::cout << "RightBrace not consumed" << std::endl;
        }
        auto asts = ast->Vectorize();
        return new BracesAst(asts);
    }
    z2h::Ast * SotaParser::BracketsNud(z2h::Token *token) {
        auto rb = symbolmap[SymbolType::RightBracket];
        auto ast = Expression();
        if (!this->Consume(rb)) {
            std::cout << "RightBracket not consumed" << std::endl;
        }
        auto asts = ast->Vectorize();
        return new BracketsAst(asts);
    }
    z2h::Ast * SotaParser::RegexMatchNud(z2h::Token *token) {
        auto right = Expression();
        std::cout << "RegexMatchNud: token=" << *token << " right=" << *right << std::endl;
        auto asts = right->Vectorize();
        for (auto ast : asts) {
            std::cout << *ast << std::endl;
        }
        std::cout << "here" << std::endl;
        return new RegexMatchAst(token, asts[0], asts[1], asts[2]);
    }
    z2h::Ast * SotaParser::RegexReplaceNud(z2h::Token *token) {
        auto right = Expression();
        std::cout << "RegexReplaceNud: token=" << *token << " right=" << *right << std::endl;
        auto asts = right->Vectorize();
        for (auto ast : asts) {
            std::cout << *ast << std::endl;
        }
        std::cout << "here" << std::endl;
        return new RegexReplaceAst(token, asts[0], asts[1], asts[2], asts[3]);
    }
    z2h::Ast * SotaParser::IfThenElifElseNud(z2h::Token *token) {
        return nullptr;
    }

    // led parsing functions
    z2h::Ast * SotaParser::NullptrLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
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
    z2h::Ast * SotaParser::CommaLed(z2h::Ast *left, z2h::Token *token) {
        auto right = Expression(token->symbol->lbp);
        return new z2h::BinaryAst(token, left, (right ? right : new NullAst()));
    }
    z2h::Ast * SotaParser::AssignLed(z2h::Ast *left, z2h::Token *token) {
        z2h::Ast *right = this->Expression(token->symbol->lbp -1 ); //right associative?
        auto asts = left->Vectorize();
        if (asts.size() > 1) {
            auto ast = new AssignAst(token, new z2h::VectorAst(asts), right);
            delete left;
            return ast;
        }
        return new AssignAst(token, left, right);
    }
    z2h::Ast * SotaParser::FuncLed(z2h::Ast *left, z2h::Token *token) {
        z2h::Ast *right = this->Expression();
        return new FuncAst(token, left, right);
    }
    z2h::Ast * SotaParser::CallLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::TernaryLed(z2h::Ast *left, z2h::Token *token) {
        auto action = Expression();
        auto pair = ConditionalAst::Pair(left, action);
        auto expected = symbolmap[SotaParser::SymbolType::Colon];
        if (nullptr == Consume(expected))
            throw SotaException(__FILE__, __LINE__, "colon : expected");
        auto defaultAction = Expression();
        return new ConditionalAst(token, {pair}, defaultAction);
    }
    z2h::Ast * SotaParser::IfThenElseLed(z2h::Ast *left, z2h::Token *token) {
        auto predicate = Expression();
        auto pair = ConditionalAst::Pair(predicate, left);
        auto expected = symbolmap[SotaParser::SymbolType::Else];
        if (nullptr == Consume(expected))
            throw SotaException(__FILE__, __LINE__, "else expected");
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
