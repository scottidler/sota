#include "ast.h"
#include "token.h"
#include "symbol.h"

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

    std::string InfixAst::Print() { return "(" + op.Value() + " " + left->Print() + " " + right->Print() + ")"; }
    InfixAst::~InfixAst() {
        if (left)
            delete left;
        if (right)
            delete right;
    }
    InfixAst::InfixAst(Token op, Ast *left, Ast *right)
        : op(op)
        , left(left)
        , right(right) {
    }

    std::string PrefixAst::Print() { return "(" + op.Value() + " " + right->Print() + ")"; }
    PrefixAst::~PrefixAst() {
        if (right)
            delete right;
    }
    PrefixAst::PrefixAst(Token op, Ast *right)
        : op(op)
        , right(right) {
    }

    std::string ConditionalAst::Pair::Print() { return ""; }
    ConditionalAst::Pair::~Pair() {
        if (predicate)
            delete predicate;
        if (block)
            delete block;
    }
    ConditionalAst::Pair::Pair(Ast *predicate, Ast *block)
        : predicate(predicate)
        , block(block) {
    }

    std::string ConditionalAst::Print() { return ""; }
    ConditionalAst::~ConditionalAst() {
        if (elseBlock)
            delete elseBlock;
    }
    ConditionalAst::ConditionalAst(std::initializer_list<Pair> pairs)
        : pairs(pairs) {
    }
    ConditionalAst::ConditionalAst(std::initializer_list<Pair> pairs, Ast *elseBlock)
        : pairs(pairs)
        , elseBlock(elseBlock) {
    }
}
