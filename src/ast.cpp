#include "ast.h"
#include "token.h"
#include "symbol.h"

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

    std::string InfixAst::Print() { return "(" + op.Value() + " " + left->Print() + " " + right->Print() + ")"; }
    InfixAst::~InfixAst() {
        if (nullptr != left)
            delete left;
        if (nullptr != right)
            delete right;
    }
    InfixAst::InfixAst(Token op, Ast *left, Ast *right)
        : op(op)
        , left(left)
        , right(right) {
    }

    std::string PrefixAst::Print() { return "(" + op.Value() + " " + right->Print() + ")"; }
    PrefixAst::~PrefixAst() {
        if (nullptr != right)
            delete right;
    }
    PrefixAst::PrefixAst(Token op, Ast *right)
        : op(op)
        , right(right) {
    }

    std::string ConditionalAst::Pair::Print() { return "(pair " + predicate->Print() + " " + action->Print() + ")"; }

    /*
    ConditionalAst::Pair::~Pair() {
        std::cout << "~Pair called" << std::endl;
        std::cout << "Pair:" << std::endl;
        if (nullptr != predicate) {
            delete predicate;
        }
        std::cout << "before action test: " << action->Print() << std::endl;
        if (nullptr != action) {
            std::cout << "before action delete" << std::endl;
            delete action;
        }
        std::cout << "after predicate and action delete" << std::endl;
    }
    */
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
