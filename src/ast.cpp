#include "ast.h"
#include "token.h"

#include <string>

namespace sota {

    std::string NewlineAst::Print() { return "(nl " + value + ")"; }
    NewlineAst::~NewlineAst() {}
    NewlineAst::NewlineAst(std::string value)
        : value(value) {
    }

    std::string IdentifierAst::Print() { return "(id " + value + ")"; }
    IdentifierAst::~IdentifierAst() {}
    IdentifierAst::IdentifierAst(std::string value)
        : value(value) {
    }

    std::string NumberAst::Print() { return "(num " + value + ")"; }
    NumberAst::~NumberAst() {}
    NumberAst::NumberAst(std::string value)
        : value(value) {
    }

    std::string InfixOperatorAst::Print() { return "(" + op.Value() + " " + left->Print() + " " + right->Print() + ")"; }
    InfixOperatorAst::~InfixOperatorAst() {
        if (left)
            delete left;
        if (right)
            delete right;
    }
    InfixOperatorAst::InfixOperatorAst(Token op, Ast *left, Ast *right)
        : op(op)
        , left(left)
        , right(right) {
    }
        
}
