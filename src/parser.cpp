#include "parser.h"

#include "ast.h"
#include "utils.h"
#include "token.h"

#include "exceptions.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace sota {

    std::string Parser::Load(const std::string &filename) {
        if (!exists(filename))
            SotaException(filename + " doesn't exist or is unreadable");
        std::ifstream file(filename);
        std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return text;
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

        if (Index < Source.length()) {
            std::cout << "Scan: Index=" << Index << std::endl;
            size_t end = Index;
            Symbol *match = nullptr;
            for (auto kvp : Symbols) {
                auto symbol = kvp.second;
                size_t index = symbol->Scan(symbol, Source.substr(Index, Source.length() - Index), Index);
                if (index > end || (match != nullptr && symbol->LBP > match->LBP && index == end)) {
                    match = symbol;
                    end = index;
                }
            }
            if (Index == end)
                throw SotaException("Parser::Scan: invalid symbol");
            return Token(match, Source, Index, end - Index);
        }
        return Token();
    }

    Token Parser::LookAhead(size_t distance) {
        auto token = Token();
        for (size_t i = 0; i < distance; ++i) {
            token = Scan();
            ++Index;
        }
        return token;
    }

    /*public*/

    Parser::Parser(const Types2Symbols &symbols)
        : Symbols(symbols)
        , Source("")
        , Index(0) {}

    Ast * Parser::ParseFile(const std::string &filename) {
        auto source = Load(filename);
        return Parse(source);
    }

    Ast * Parser::Parse(std::string source) {
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
        auto token = LookAhead(1);
        return token;
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
