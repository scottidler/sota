#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "z2h/ast.hpp"
#include "z2h/token.hpp"

namespace sota {

    inline void sepby(std::ostream &os, std::string sep, std::vector<z2h::Ast *> items) {
        if (items.size()) {
            size_t i = 0;
            for (; i < items.size() - 1; ++i) {
                os << *items[i] << sep;
            }
            os << *items[i];
        }
    }
/*
    struct NullAst : public z2h::Ast {
        ~NullAst() {}
        NullAst()
            : z2h::Ast(nullptr) {}
    protected:
        void Print(std::ostream &os) const {
            os << "( )";
        }
    };
*/

    struct IdentifierAst : public z2h::ValueAst {
        ~IdentifierAst() {}
        IdentifierAst(z2h::Token *token)
            : z2h::ValueAst(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(id " + token->value + ")";
        }
    };

    struct NumberAst : public z2h::ValueAst {
        ~NumberAst() {}
        NumberAst(z2h::Token *token)
            : z2h::ValueAst(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(num " + token->value + ")";
        }
    };
/*
    struct ExpressionsAst : public z2h::Ast {
        std::vector<z2h::Ast *> expressions;

        ~ExpressionsAst() {}
        ExpressionsAst(std::vector<z2h::Ast *> expressions) {
            for (auto expression : expressions)
                this->expressions.push_back(expression);
        }

    protected:
        void Print(std::ostream &os) const {
            os << "( ";
            sepby(os, " ", expressions);
            os << ")";
        }
    };
*/

    struct CommaAst : public z2h::BinaryAst {
        ~CommaAst() {}
        CommaAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct ParensAst : public z2h::VectorAst {
        ~ParensAst() {}
        ParensAst(std::vector<z2h::Ast *> asts)
            : z2h::VectorAst("()", asts) {}
    };

    struct BracesAst : public z2h::VectorAst {
        ~BracesAst() {}
        BracesAst(std::vector<z2h::Ast *> asts)
            : z2h::VectorAst("{}", asts) {}
    };

    struct BracketsAst : public z2h::VectorAst {
        ~BracketsAst() {}
        BracketsAst(std::vector<z2h::Ast *> asts)
            : z2h::VectorAst("[]", asts) {}
    };

    struct InfixAst : public z2h::BinaryAst {
        ~InfixAst() {}
        InfixAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct PrefixAst : public z2h::ValueAst {
        z2h::Ast *right;

        ~PrefixAst() {}
        PrefixAst(z2h::Token *token, z2h::Ast *right)
            : z2h::ValueAst(token)
            , right(right) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + token->value + " " << *right << ")";
        }
    };

    struct AssignAst : public z2h::BinaryAst {
        ~AssignAst() {}
        AssignAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct FuncAst : public z2h::BinaryAst {
        ~FuncAst() {}
        FuncAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct RegexMatchAst : public z2h::ValueAst {
        z2h::Ast *string;
        z2h::Ast *pattern;
        z2h::Ast *options;

        ~RegexMatchAst() {}
        RegexMatchAst(z2h::Token *token, z2h::Ast *string, z2h::Ast *pattern, z2h::Ast *options = nullptr)
            : z2h::ValueAst(token)
            , string(string)
            , pattern(pattern)
            , options(options) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + token->value << " " << *string << " " << *pattern;
            if (options)
                os << " " << *options;
            os << ")";
        }
    };

    struct RegexReplaceAst : public z2h::ValueAst {
        z2h::Ast *string;
        z2h::Ast *pattern;
        z2h::Ast *replace;
        z2h::Ast *options;

        ~RegexReplaceAst() {}
        RegexReplaceAst(z2h::Token *token, z2h::Ast *string, z2h::Ast *pattern, z2h::Ast *replace, z2h::Ast *options = nullptr)
            : z2h::ValueAst(token)
            , string(string)
            , pattern(pattern)
            , replace(replace)
            , options(options) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + token->value << " " << *string << " " << *pattern << " " << *replace;
            if (options)
                os << " " << *options;
            os << ")";
        }
    };

    struct ConditionalAst : public z2h::ValueAst {

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
            : z2h::ValueAst(token)
            , pairs(pairs)
            , action(nullptr) {}
        ConditionalAst(z2h::Token *token, std::initializer_list<Pair> pairs, z2h::Ast *action)
            : z2h::ValueAst(token)
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

    struct BlockAst : public z2h::VectorAst {
        ~BlockAst() {}
        BlockAst(std::vector<Ast *> asts)
            : z2h::VectorAst(asts) {}
    protected:
        void Print(std::ostream &os) const {
            os << "{ ";
            sepby(os, "; ", asts);
            os << "}";
        }
    };
}
#endif /*__SOTA_AST__*/
