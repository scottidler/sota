#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>

namespace sota {

    class Ast {
    public:
        virtual std::string Print() = 0;
        virtual ~Ast() {}
        Ast() {}
    };

    class Token;

    class Identifier : public Ast {
    public:
        std::string Value;

        std::string Print();
        ~Identifier();
        Identifier(std::string value);
    };

    class InfixOperator : public Ast {
    public:
        Token *Op;
        Ast *Left;
        Ast *Right;

        std::string Print();
        ~InfixOperator();
        InfixOperator(Token *op, Ast *left, Ast *right);
    };
}
#endif /*__SOTA_AST__*/
