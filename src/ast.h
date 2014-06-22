#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>

#include "token.h"

namespace sota {

    class Ast {
        public:
        virtual std::string Print() = 0;
        virtual ~Ast() = 0;
        Ast() {}
    };

    class Token;
    class InfixOperator : public Ast {
        public:
        std::string Print() { return "InfixOperator"; }
        ~InfixOperator() {
            if (Left)
                delete Left;
            if (Right)
                delete Right;
        }
        InfixOperator(Token op, Ast *left, Ast *right):
            Op(op), Left(left), Right(right) {
        }
        
        Token Op;
        Ast *Left;
        Ast *Right;

    };
}
#endif /*__SOTA_AST__*/
