#include "parser.h"

#include "ast.h"
#include "token.h"
#include "symbol.h"

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
        Token token = Token(nullptr, _source, _index, 0);
        if (_tokens.size()) {
            token = _tokens.front();
            _tokens.pop_front();
        }
        return token;
    }

    Token Parser::Scan() {

        Symbol *match = nullptr;
        if (_index < _source.length()) {
            auto end = _index;
            std::vector<Symbol *> symbols;
            for (auto symbol : symbols) {
                auto index = symbol->Scan(_source, _index);
                if (index > end || (match != nullptr && symbol->LBP > match->LBP && index == end)) {
                    match = symbol;
                    end = index;
                }
            }
            if (_index == end)
                throw SotaException("Parser::Scan: invalid symbol");
        }
        return Token();
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

    Parser::Parser() {}

    Parser::Parser(const std::string &source)
        : _source(source) {}

    Ast * Parser::ParseFile(const std::string &filename) {
        auto source = Load(filename);
        return Parse(source);
    }

    Ast * Parser::Parse(const std::string &source) {
        _source = source;
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
        return Token(symbol, _source, 0, 0);
    }

    Token Parser::Consume(const SymbolType &expected, const std::string &message) {
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
