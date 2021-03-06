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

    struct NullAst : public z2h::Ast {
        ~NullAst() {}
        NullAst()
            : z2h::Ast(nullptr) {}
    protected:
        void Print(std::ostream &os) const {
            os << "( )";
        }
    };

    struct IdentifierAst : public z2h::Ast {
        ~IdentifierAst() {}
        IdentifierAst(z2h::Token *token)
            : z2h::Ast(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(id " + value + ")";
        }
    };

    struct NumberAst : public z2h::Ast {
        ~NumberAst() {}
        NumberAst(z2h::Token *token)
            : z2h::Ast(token) {}
    protected:
        void Print(std::ostream &os) const {
            os << "(num " + value + ")";
        }
    };

    struct EoesAst : public z2h::VectorAst {
        ~EoesAst() {}
        EoesAst(std::vector<z2h::Ast *> asts)
            : z2h::VectorAst(",", asts) {}
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

    struct CommaAst : public z2h::BinaryAst {
        ~CommaAst() {}
        CommaAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    protected:
        void Print(std::ostream &os) {
        }
    };

    struct InfixAst : public z2h::BinaryAst {
        ~InfixAst() {}
        InfixAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
        virtual std::vector<z2h::Ast *> Vectorize() {
            return { this };
        }
    };

    struct KvpAst : public z2h::BinaryAst {
        ~KvpAst() {}
        KvpAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct AssignAst : public InfixAst {
        ~AssignAst() {}
        AssignAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : InfixAst(token, left, right) {}
    };

    struct PrefixAst : public z2h::Ast {
        z2h::Ast *right;

        ~PrefixAst() {}
        PrefixAst(z2h::Token *token, z2h::Ast *right)
            : z2h::Ast(token)
            , right(right) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + value + " " << *right << ")";
        }
    };

    struct ArrowAst : public z2h::BinaryAst {
        ~ArrowAst() {}
        ArrowAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct CallAst : public z2h::BinaryAst {
        ~CallAst() {}
        CallAst(z2h::Token *token, z2h::Ast *left, z2h::Ast *right)
            : z2h::BinaryAst(token, left, right) {}
    };

    struct RegexMatchAst : public z2h::Ast {
        z2h::Ast *string;
        z2h::Ast *pattern;
        z2h::Ast *options;

        ~RegexMatchAst() {}
        RegexMatchAst(z2h::Token *token, z2h::Ast *string, z2h::Ast *pattern, z2h::Ast *options = nullptr)
            : z2h::Ast(token)
            , string(string)
            , pattern(pattern)
            , options(options) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + value << " " << *string << " " << *pattern;
            if (options)
                os << " " << *options;
            os << ")";
        }
    };

    struct RegexReplaceAst : public z2h::Ast {
        z2h::Ast *string;
        z2h::Ast *pattern;
        z2h::Ast *replace;
        z2h::Ast *options;

        ~RegexReplaceAst() {}
        RegexReplaceAst(z2h::Token *token, z2h::Ast *string, z2h::Ast *pattern, z2h::Ast *replace, z2h::Ast *options = nullptr)
            : z2h::Ast(token)
            , string(string)
            , pattern(pattern)
            , replace(replace)
            , options(options) {}

    protected:
        void Print(std::ostream &os) const {
            os << "(" + value << " " << *string << " " << *pattern << " " << *replace;
            if (options)
                os << " " << *options;
            os << ")";
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

    struct BlockAst : public z2h::VectorAst {
        ~BlockAst() {}
        BlockAst(std::vector<z2h::Ast *> statements)
            : z2h::VectorAst("", statements) {}

    protected:
        void Print(std::ostream &os) const {
            os << "{ ";
            sepby(os, "; ", asts);
            os << " }";
        }
    };

}
#endif /*__SOTA_AST__*/
