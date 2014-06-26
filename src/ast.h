#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>

namespace sota {

    class Token;

    class Ast {
    public:
        virtual std::string print() = 0;
        virtual ~Ast() {}
        Ast() {}
    };

    class IdentifierAst : public Ast {
    public:
        std::string value;

        std::string print();
        ~IdentifierAst();
        IdentifierAst(std::string value);
    };

    class InfixOperator : public Ast {
    public:
        Token *op;
        Ast *left;
        Ast *right;

        std::string print();
        ~InfixOperator();
        InfixOperator(Token *op, Ast *left, Ast *right);
    };
}
#endif /*__SOTA_AST__*/
