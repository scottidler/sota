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
        Token token = Token();
        if (_tokens.size()) {
            token = _tokens.front();
            _tokens.pop_front();
        }
        return token;
    }

    Token Parser::Scan() {

        if (index < source.length()) {
            size_t end = index;
            Symbol *match = nullptr;
            for (auto kvp : symbols) {
                auto symbol = kvp.second;
                size_t pos = symbol->scan(symbol, source.substr(index, source.length() - index), index);
                if (pos > end || (match != nullptr && symbol->lbp > match->lbp && pos == end)) {
                    match = symbol;
                    end = pos;
                }
            }
            if (index == end)
                throw SotaException("Parser::Scan: invalid symbol");
            return Token(*match, source, index, end - index);
        }
        return Token();
    }

    /*public*/

    Parser::Parser(const Types2Symbols &symbols)
        : symbols(symbols)
        , source("")
        , index(0) {}

    Ast * Parser::ParseFile(const std::string &filename) {
        auto source = Load(filename);
        return Parse(source);
    }

    Ast * Parser::Parse(std::string source) {
        this->source = source;
        return Parse();
    }

    Ast * Parser::Parse(size_t lbp/* = 0 */) {

        Token curr = Consume();
        std::cout << "curr: " << curr << std::endl;

        Ast *left = curr.Nud(this, &curr);

        Token next = LookAhead(1);
        std::cout << "next: " << next << std::endl;
        std::cout << "lbp: " << lbp << " next.symbol.lbp: " << next.symbol.lbp << std::endl;

        while (lbp < next.symbol.lbp) {
            std::cout << "lbp test passed" << std::endl;
            Token next  = Consume();
            left = next.Led(this, left, &next);
        }

        return left;
    }

    Token Parser::LookAhead(size_t distance) {
        if (distance == 0) {
            return Scan();
        }

        Token token;
        while(distance > _tokens.size()) {
            token = Take(Scan());
        }

        return token;
    }

    Token Parser::Consume() {
        auto la = LookAhead(1);
        index += la.length;
        return   Emit();
    }

    Token Parser::Consume(const size_t &expected, const std::string &message) {
        auto token = Consume();
        if (token.symbol.type != expected)
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
