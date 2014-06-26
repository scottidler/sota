#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>

namespace sota {

    class Token;

    class Ast {
    public:
        virtual std::string Print() = 0;
        virtual ~Ast() {}
        Ast() {}
    };

    class IdentifierAst : public Ast {
    public:
        std::string value;

        std::string Print();
        ~IdentifierAst();
        IdentifierAst(std::string value);
    };

    class InfixOperatorAst : public Ast {
    public:
        Token *op;
        Ast *left;
        Ast *right;

        std::string Print();
        ~InfixOperatorAst();
        InfixOperatorAst(Token *op, Ast *left, Ast *right);
    };
}
#endif /*__SOTA_AST__*/
