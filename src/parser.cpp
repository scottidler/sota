#include "parser.h"

#include "ast.h"
#include "token.h"
#include "symbol.h"

namespace sota {

    Token Parser::Consume() {
        auto symbol = new Symbol(SymbolType::Add, "+", nullptr, nullptr, nullptr, 0);
        return Token(symbol, _source, 0, 0);
    }

    /*public*/

    Parser::Parser() {}

    Parser::Parser(const std::string &source)
        : _source(source) {}
    
    Ast * Parser::ParseFile(const std::string &filename) {
        return Parse("");
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
}
