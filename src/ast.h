#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>
#include <vector>

#include "z2h/token.hpp"

namespace sota {

    class Ast {
    public:
        virtual std::string Print() = 0;
        virtual ~Ast() {}
        Ast() {}
    };

    class NewlineAst : public Ast {
    public:
        std::string value;

        std::string Print();
        ~NewlineAst();
        NewlineAst(std::string value);
    };

    class IdentifierAst : public Ast {
    public:
        std::string value;

        std::string Print();
        ~IdentifierAst();
        IdentifierAst(std::string value);
    };

    class NumberAst : public Ast {
    public:
        std::string value;

        std::string Print();
        ~NumberAst();
        NumberAst(std::string value);
    };

    class InfixAst : public Ast {
    public:
        z2h::Token<Ast *> *op;
        Ast *left;
        Ast *right;

        std::string Print();
        ~InfixAst();
        InfixAst(z2h::Token<Ast *> *op, Ast *left, Ast *right);
    };

    class PrefixAst : public Ast {
    public:
        z2h::Token<Ast *> *op;
        Ast *right;

        std::string Print();
        ~PrefixAst();
        PrefixAst(z2h::Token<Ast *> *op, Ast *right);
    };

    class ConditionalAst : public Ast {
    public:
        typedef struct Pair {
            Ast *predicate;
            Ast *action;

            std::string Print();
            Pair(Ast *predicate, Ast *action);
        } Pair;

        std::vector<Pair> pairs;
        Ast *action;

        std::string Print();
        ~ConditionalAst();
        ConditionalAst(std::initializer_list<Pair> pairs);
        ConditionalAst(std::initializer_list<Pair> pairs, Ast *action);
    };
}
#endif /*__SOTA_AST__*/
