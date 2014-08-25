#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>
#include <vector>
#include <iostream>

#include "z2h/ast.hpp"
#include "z2h/token.hpp"

namespace sota {

    struct NewlineAst : public z2h::Ast {
        ~NewlineAst() {}
        NewlineAst(z2h::Token *token)
            : z2h::Ast(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(nl " + token->value + ")"; 
        }
    };

    struct IdentifierAst : public z2h::Ast {
        ~IdentifierAst() {}
        IdentifierAst(z2h::Token *token)
            : z2h::Ast(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(id " + token->value + ")";
        }
    };

    struct NumberAst : public z2h::Ast {
        ~NumberAst() {}
        NumberAst(z2h::Token *token)
            : z2h::Ast(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(num " + token->value + ")";
        }
    };

    struct ExpressionsAst : public z2h::Ast {
        std::vector<z2h::Ast *> expressions;

        ~ExpressionsAst() {}
        ExpressionsAst(std::vector<z2h::Ast *> expressions) {
            for (auto expression : expressions)
                this->expressions.push_back(expression);
        }

    protected:
        void Print(std::ostream &os) const {
            os << "(el ";
            for (auto expression : expressions) {
                os << *expression << ", ";
            }
            os << ")";
        }
    };

    struct InfixAst : public z2h::Ast {
        z2h::Ast *left;
        z2h::Ast *right;

        ~InfixAst() {}
        InfixAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::Ast(token)
            , left(left)
            , right(right) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + token->value + " " << *left << " " << *right << ")";
        }
    };

    struct PrefixAst : public z2h::Ast {
        z2h::Ast *right;

        ~PrefixAst() {}
        PrefixAst(z2h::Token *token, z2h::Ast *right)
            : z2h::Ast(token)
            , right(right) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + token->value + " " << *right << ")";
        }
    };

    struct AssignAst : public z2h::Ast {
        z2h::Ast *left;
        z2h::Ast *right;

        ~AssignAst() {}
        AssignAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::Ast(token)
            , left(left)
            , right(right) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + token->value + " " << *left << " " << *right << ")";
        }
    };

    struct FuncAst : public z2h::Ast {
        z2h::Ast *args;
        z2h::Ast *body;

        ~FuncAst() {}
        FuncAst(z2h::Token *token, z2h::Ast *args, z2h::Ast *body)
            : z2h::Ast(token)
            , args(args)
            , body(body) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(func " << *args << " " << *body << ")";
        }
    };

    struct ConditionalAst : public z2h::Ast {

        struct Pair {
            z2h::Ast *predicate;
            z2h::Ast *action;

            Pair(z2h::Ast *predicate, z2h::Ast *action)
                : predicate(predicate)
                , action(action) {}

        protected:
            friend std::ostream & operator <<(std::ostream &os, const Pair &pair) {
                os << "(pair " << *pair.predicate << " " << *pair.action << ")";
                return os;
            }
        };

        std::vector<Pair> pairs;
        z2h::Ast *action;

        ~ConditionalAst() {}
        ConditionalAst(z2h::Token *token, std::initializer_list<Pair> pairs)
            : z2h::Ast(token)
            , pairs(pairs)
            , action(nullptr) {}
        ConditionalAst(z2h::Token *token, std::initializer_list<Pair> pairs, z2h::Ast *action)
            : z2h::Ast(token)
            , pairs(pairs)
            , action(action) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(cond";
            for (auto pair : pairs) {
                os << " " << pair;
            }
            if  (nullptr != action)
                os << " " << *action;
            os << ")";
        }
    };
}
#endif /*__SOTA_AST__*/
