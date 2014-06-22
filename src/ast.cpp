#include "ast.h"
#include "token.h"

namespace sota {

    std::string InfixOperator::Print() { return "InfixOperator"; }
    InfixOperator::~InfixOperator() {
        if (Left)
            delete Left;
        if (Right)
            delete Right;
    }
    InfixOperator::InfixOperator(Token *op, Ast *left, Ast *right)
        : Op(op)
        , Left(left)
        , Right(right) {
    }
        
}
