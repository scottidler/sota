#include "z2h/token.hpp"
#include "z2h/symbol.hpp"
#include "z2h/lexer.hpp"
#include "z2h/parser.hpp"

#include "ast.hpp"
#include "lexer.h"
#include "parser.h"
#include "bindpower.h"
#include "exceptions.h"

#include <map>
#include <vector>
#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>

namespace sota {

    z2h::Ast * SotaParser::Parse() {
        auto statements = Statements();
        return new BlockAst(statements);
    }

    std::vector<z2h::Symbol *> SotaParser::Symbols() {
        return symbols;
    }
    std::vector<z2h::Token *> SotaParser::Tokenize() {
        index = 0;
        z2h::Token *token = nullptr;
        do {
            token = Consume();
        } while (TokenType::EndOfFile != token->type);
        return tokens;
    }

    std::vector<z2h::Ast *> SotaParser::Expressions(z2h::Token *end) {
        std::vector<z2h::Ast *> expressions;
        auto la1 = LookAhead1();
        while (end != la1) {
            auto expression = Expression();
            expressions.push_back(expression);
            if (!Consume(TokenType::EndOfExpression)) {
                break;
            }
            la1 = LookAhead1();
        }
        return expressions;
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
        auto right = Expression(GetSymbol(*token)->lbp);
        return new z2h::BinaryAst(token, new NullAst(), right);
    }
    z2h::Ast * SotaParser::ParensNud(z2h::Token *token) {
        nesting.Push(token);
        auto eq = GetSymbol(TokenType::Assign);
        eq->lbp += 10;
        auto ast = Expression();
        if (!Consume(TokenType::RightParen)) {
            std::cout << "RightParen not consumed" << std::endl;
        }
        eq->lbp -= 10;
        nesting.Pop(token);
        if (ast) {
            if (ast->token->value == ",") {
                auto asts = ast->Vectorize();
                return new ParensAst(asts);
            }
        }
        else
            return new NullAst();
        
        return ast;
    }
    z2h::Ast * SotaParser::BracesNud(z2h::Token *token) {
        auto statements = Statements();
        if (!Consume(TokenType::RightBrace)) {
            std::cout << "RightBrace not consumed" << std::endl;
        }
        return new BlockAst(statements);
    }
    z2h::Ast * SotaParser::BracketsNud(z2h::Token *token) {
        auto ast = Expression();
        if (!Consume(TokenType::RightBracket)) {
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
        return new RegexMatchAst(token, asts[0], asts[1], asts[2]);
    }
    z2h::Ast * SotaParser::RegexReplaceNud(z2h::Token *token) {
        auto right = Expression();
        std::cout << "RegexReplaceNud: token=" << *token << " right=" << *right << std::endl;
        auto asts = right->Vectorize();
        for (auto ast : asts) {
            std::cout << *ast << std::endl;
        }
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
        z2h::Ast *right = Expression(GetSymbol(*token)->lbp);
        return new InfixAst(token, left, right);
    }
    z2h::Ast * SotaParser::PostfixLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::CommaLed(z2h::Ast *left, z2h::Token *token) {
        auto right = Expression(GetSymbol(*token)->lbp);
        return new z2h::BinaryAst(token, left, (right ? right : new NullAst()));
    }
    z2h::Ast * SotaParser::ParensLed(z2h::Ast *left, z2h::Token *token) {
        nesting.Push(token);
        auto eq = GetSymbol(TokenType::Assign);
        eq->lbp += 10;
        auto ast = Expression();
        if (!Consume(TokenType::RightParen)) {
            std::cout << "RightParen not consumed" << std::endl;
        }
        eq->lbp -= 10;
        nesting.Pop(token);
        if (ast) {
            auto asts = ast->Vectorize();
            return new CallAst(token, left, new ParensAst(asts));
        }
        return new CallAst(token, left, new NullAst());
    }
    z2h::Ast * SotaParser::BracesLed(z2h::Ast *left, z2h::Token *token) {
        auto ast = Expression();
        if (!Consume(TokenType::RightBrace)) {
            std::cout << "RightBrace not consumed" << std::endl;
        }
        auto asts = ast->Vectorize();
        return new CallAst(token, left, new ParensAst(asts));
    }
    z2h::Ast * SotaParser::BracketsLed(z2h::Ast *left, z2h::Token *token) {
        auto ast = Expression();
        if (!Consume(TokenType::RightBracket)) {
            std::cout << "RightBracket not consumed" << std::endl;
        }
        auto asts = ast->Vectorize();
        return new CallAst(token, left, new ParensAst(asts));
    }
    z2h::Ast * SotaParser::AssignLed(z2h::Ast *left, z2h::Token *token) {
        z2h::Ast *right = Expression(GetSymbol(*token)->lbp -1 ); //right associative?
        auto asts = left->Vectorize();
        if (asts.size() > 1) {
            auto ast = new AssignAst(token, new z2h::VectorAst(asts), right);
            delete left;
            return ast;
        }
        return new AssignAst(token, left, right);
    }
    z2h::Ast * SotaParser::FuncLed(z2h::Ast *left, z2h::Token *token) {
        z2h::Ast *right = Expression();
        return new FuncAst(token, left, right);
    }
    z2h::Ast * SotaParser::CallLed(z2h::Ast *left, z2h::Token *token) {
        return nullptr;
    }
    z2h::Ast * SotaParser::TernaryLed(z2h::Ast *left, z2h::Token *token) {
        auto action = Expression();
        auto pair = ConditionalAst::Pair(left, action);
        if (nullptr == Consume(TokenType::Colon))
            throw SotaException(__FILE__, __LINE__, "colon : expected");
        auto defaultAction = Expression();
        return new ConditionalAst(token, {pair}, defaultAction);
    }
    z2h::Ast * SotaParser::IfThenElseLed(z2h::Ast *left, z2h::Token *token) {
        auto predicate = Expression();
        auto pair = ConditionalAst::Pair(predicate, left);
        if (nullptr == Consume(TokenType::Else))
            throw SotaException(__FILE__, __LINE__, "else expected");
        auto defaultAction = Expression();
        return new ConditionalAst(token, {pair}, defaultAction);
    }

    std::exception SotaParser::Exception(const char *file, size_t line, const std::string &message) {
        return SotaException(file, line, message);
    }

    SotaParser::SotaParser(SotaLexer *lexer)
        : z2h::Parser<SotaParser>(lexer) {
        #define T(k,b,t,n,l) new z2h::Symbol(b, STD(t), NUD(n), LED(l) ),
        symbols = {
            SYMBOLS
        };
        #undef T
    }
}
