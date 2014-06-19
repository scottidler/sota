#ifndef __SOTA_LEXER__
#define __SOTA_LEXER__ = 1

#include <queue>
#include <stack>
#include <string>
#include <vector>
#include "utils.h"
#include "token.h"
#include "stream.hpp"

namespace sota {
    class SotaLexer : public SotaStream<char> {

        unsigned int _stride;
        std::stack<unsigned int> _indents;
        std::deque<Token> _tokens;

        Token CreateToken(unsigned int index);
        //Token CreateToken(TokenType type);
        //Token CreateToken(TokenType type, unsigned int length);
        Token CreateToken(TokenType type, unsigned int length, unsigned int index);

        Token Take(Token token);
        Token Emit();

        Token EndOfLine();
        Token Dent();
        Token WhiteSpace();
        Token RangedMeta(Token token);
        Token RangedComment(Token token);
        Token LineComment(Token token);
        Token Hash();
        Token Literal();
        Token Symbol();
        Token IdentifierNumberOrKeyword();
        Token EndOfFile();

    public:
        ~SotaLexer();
        SotaLexer(std::vector<char> chars);

        unsigned int Line(const Token &token);
        unsigned int Column(const Token &token);
        //std::string Value(const Token &token) const;
        std::string Pretty(const Token &token);

        Token Scan();
        std::vector<Token> Tokenize();
    };
}

#endif /* __SOTA_LEXER__ */
