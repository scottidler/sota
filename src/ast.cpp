#include "ast.h"
#include "token.h"

#include <string>

namespace sota {

    std::string IdentifierAst::print() { return "(id " + value + ")"; }
    IdentifierAst::~IdentifierAst() {}
    IdentifierAst::IdentifierAst(std::string value)
        : value(value) {
    }

    std::string InfixOperator::print() { return "(op:" + left->print() + " " + right->print() + ")"; }
    InfixOperator::~InfixOperator() {
        if (left)
            delete left;
        if (right)
            delete right;
    }
    InfixOperator::InfixOperator(Token *op, Ast *left, Ast *right)
        : op(op)
        , left(left)
        , right(right) {
    }
        
}
