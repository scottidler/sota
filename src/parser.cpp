#include "parser.h"

#include "ast.h"
#include "token.h"

#include "exceptions.h"

#include <string>
#include <vector>

namespace sota {

    std::string Parser::Load(const std::string &filename) {
        return "";
    }

    Token Parser::Take(Token token) {
        if (token)
            _tokens.push_back(token);
        return token;
    }

    Token Parser::Emit() {
        Token token = Token(nullptr, Source, Index, 0);
        if (_tokens.size()) {
            token = _tokens.front();
            _tokens.pop_front();
        }
        return token;
    }

    Token Parser::Scan() {

        auto end = Index;
        Symbol *match = nullptr;
        if (Index < Source.length()) {
            for (auto kvp : Symbols) {
                auto symbol = kvp.second;
                auto index = symbol->Scan(Source, Index);
                if (index > end || (match != nullptr && symbol->LBP > match->LBP && index == end)) {
                    match = symbol;
                    end = index;
                }
            }
            if (Index == end)
                throw SotaException("Parser::Scan: invalid symbol");
        }
        return Token(match, Source, Index, end - Index);
    }

    Token Parser::LookAhead(int distance) {
        auto token = Token();
        while (distance--) {
            if (token = Emit())
                continue;
            token = Scan();
        }
        return token;
    }

    /*public*/

    Parser::Parser(const Types2Symbols &symbols)
        : Symbols(symbols) {}

    Ast * Parser::ParseFile(const std::string &filename) {
        auto source = Load(filename);
        return Parse(source);
    }

    Ast * Parser::Parse(const std::string &source) {
        Source = source;
        return Parse();
    }

    Ast * Parser::Parse(size_t lbp/* = 0 */) {

        Token token = Consume();

        Ast *left = token.Nud(this, &token);

        while (lbp < token.LBP()) {
            Token token = Consume();
            token.Led(this, left, &token);
        }

        return left;
    }

    Token Parser::Consume() {
        auto symbol = new Symbol(SymbolType::Add, "+", nullptr, nullptr, nullptr, 0);
        return Token(symbol, Source, 0, 0);
    }

    Token Parser::Consume(const size_t &expected, const std::string &message) {
        auto token = Consume();
        if (token.Type() != expected)
            throw SotaException(message);
        return token;
    }

    size_t Parser::Line() {
        return 0;
    }

    size_t Parser::Column() {
        return 0;
    }
}
