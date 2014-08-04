#include "ast.h"

#include "z2h/token.hpp"
#include "z2h/symbol.hpp"

#include <string>
#include <typeinfo>

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

    std::string InfixAst::Print() { return "(" + op->value + " " + left->Print() + " " + right->Print() + ")"; }
    InfixAst::~InfixAst() {
        if (nullptr != left)
            delete left;
        if (nullptr != right)
            delete right;
    }
    InfixAst::InfixAst(z2h::Token<Ast *> *op, Ast *left, Ast *right)
        : op(op)
        , left(left)
        , right(right) {
    }

    std::string PrefixAst::Print() { return "(" + op->value + " " + right->Print() + ")"; }
    PrefixAst::~PrefixAst() {
        if (nullptr != right)
            delete right;
    }
    PrefixAst::PrefixAst(z2h::Token<Ast *> *op, Ast *right)
        : op(op)
        , right(right) {
    }

    std::string ConditionalAst::Pair::Print() { return "(pair " + predicate->Print() + " " + action->Print() + ")"; }

    ConditionalAst::Pair::Pair(Ast *predicate, Ast *action)
        : predicate(predicate)
        , action(action) {
    }

    std::string ConditionalAst::Print() {
        std::string result = "(cond";
        for (auto pair : pairs) {
            result += " " + pair.Print();
        }
        if (nullptr != action)
            result += " " + action->Print() + ")";
        return result;
    }
    ConditionalAst::~ConditionalAst() {
        std::cout << "ConditionalAst:" << std::endl;
        std::cout << "before action test: " << action->Print() << std::endl;
        if (nullptr != action) {
            std::cout << "before action delete" << std::endl;
            delete action;
        }
        std::cout << "after action delete" << std::endl;
    }
    ConditionalAst::ConditionalAst(std::initializer_list<Pair> pairs)
        : pairs(pairs)
        , action(nullptr) {
    }
    ConditionalAst::ConditionalAst(std::initializer_list<Pair> pairs, Ast *action)
        : pairs(pairs)
        , action(action) {
    }
}
