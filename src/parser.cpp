#include "parser.h"

#include "ast.h"
#include "utils.h"
#include "token.h"

#include "exceptions.h"

#include <cmath>
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
        //if (token)
            _tokens.push_back(token);
        return token;
    }

    Token Parser::Emit() {
        if (_tokens.size()) {
            auto token = _tokens.front();
            _tokens.pop_front();
            return token;
        }
        //throw SotaException("how?");
        return Scan();
    }

    Token Parser::Scan() {

        size_t end = index;
        Symbol *match = nullptr;
        bool skip = false;
        if (index < source.length()) {
            for (auto kvp : symbols) {
                auto symbol = kvp.second;
                long delta = symbol->Scan(symbol, source.substr(index, source.length() - index), index);
                if (delta) {
                    if (abs(delta) > 0 || (match != nullptr && symbol->lbp > match->lbp && index + abs(delta) == end)) {
                        match = symbol;
                        end = index + abs(delta);
                        skip = delta < 0;
                    }
                }
            }
            if (index == end) {
                throw SotaException("Parser::Scan: invalid symbol");
            }
            return Token(*match, source, index, end - index, skip);
        }
        return Token(); //eof
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

    Ast * Parser::Parse(size_t rbp/* = 0 */) {

        Token curr = Consume();
        if (curr.symbol.type == SymbolType::EndOfFile)
            return nullptr;

        Ast *left = curr.Nud(this, &curr);

        Token next = LookAhead(1);
        if (next.symbol.type == SymbolType::EndOfFile)
            return left;

        while (rbp < next.symbol.lbp) {
            next  = Consume();
            if (next.symbol.type == SymbolType::EndOfFile)
                return left;

            left = next.Led(this, left, &next);
        }

        return left;
    }

    Token Parser::LookAhead(size_t distance) {
        if (distance == 0)
            return Curr;

        Token token;
        while(distance > _tokens.size()) {
            auto token = Scan();
            if (token.skip) {
                index += token.length;
                continue;
            }
            Take(Scan());
        }

        return Emit();
    }

    Token Parser::Consume() {
        auto la1 = LookAhead(1);
        Curr = Emit();
        index += Curr.length;
        return Curr;
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
