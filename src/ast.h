#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>

#include "token.h"

namespace sota {

    class Token;

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
        Token op;
        Ast *left;
        Ast *right;

        std::string Print();
        ~InfixAst();
        InfixAst(Token op, Ast *left, Ast *right);
    };

    class PrefixAst : public Ast {
    public:
        Token op;
        Ast *right;

        std::string Print();
        ~PrefixAst();
        PrefixAst(Token op, Ast *right);
    };

    class ConditionalAst : public Ast {
    public:
        typedef struct Pair {
            Ast *predicate;
            Ast *block;

            std::string Print();
            ~Pair();
            Pair(Ast *predicate, Ast *block);
        } Pair;

        std::vector<Pair> pairs;
        Ast *elseBlock;

        std::string Print();
        ~ConditionalAst();
        ConditionalAst(std::initializer_list<Pair> pairs);
        ConditionalAst(std::initializer_list<Pair> pairs, Ast *elseBlock);
    };
}
#endif /*__SOTA_AST__*/
