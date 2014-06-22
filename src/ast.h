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
    class InfixOperator : public Ast {
        public:
        std::string Print();
        ~InfixOperator();
        InfixOperator(Token *op, Ast *left, Ast *right);

        Token *Op;
        Ast *Left;
        Ast *Right;

    };
}
#endif /*__SOTA_AST__*/
