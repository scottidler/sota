#ifndef __SOTA_AST__
#define __SOTA_AST__ = 1

#include <sstream>
#include <ostream>
#include "token.h"

namespace sota {

    class AstNode {
        virtual void Print(std::ostream &out) const = 0;
        public:
        virtual ~AstNode() {}
        virtual void CodeGen() = 0;
        friend std::ostream & operator<<(std::ostream &out, const AstNode &node) {
            node.Print(out);
            return out;
        }
    };

    typedef std::shared_ptr<AstNode> AstNodePtr;
    typedef std::vector<std::shared_ptr<AstNode>> AstNodePtrs;

    class InfixOperatorNode : public AstNode {

        void Print(std::ostream &out) const {
            out << "( " << *Left <<  Op << *Right << " )";
        }

        public:

        Token Op;
        AstNodePtr Left;
        AstNodePtr Right;

        ~InfixOperatorNode() {}
        InfixOperatorNode(Token op, AstNode *left, AstNode *right) : Op(op), Left(left), Right(right) {}
        void CodeGen() {}
    };

    class PrefixOperatorNode : public AstNode {

        void Print(std::ostream &out) const {
            out << "( " << Op << *Right << " )";
        }

        public:

        Token Op;
        std::unique_ptr<AstNode> Right;

        ~PrefixOperatorNode() {}
        PrefixOperatorNode(Token op, AstNode *right) : Op(op), Right(right) {}
        void CodeGen() {}
    };

    class PostfixOperatorNode : public AstNode {

        void Print(std::ostream &out) const {
            out << "( " << *Left << Op << " )";
        }

        public:

        Token Op;
        std::unique_ptr<AstNode> Left;

        ~PostfixOperatorNode() {}

        PostfixOperatorNode(Token op, AstNode *left) : Op(op), Left(left) {}
        void CodeGen() {}
    };

    class IdentifierNode : public AstNode {

        void Print(std::ostream &out) const {
            out << Name;
        }

        public:

        std::string Name;

        ~IdentifierNode() {}
        IdentifierNode(std::string name) : Name(name) {}
        void CodeGen() {}
    };

    class ConditionalNode : public AstNode {

        void Print(std::ostream &out) const {
            out << "( " << *Condition << "?" << *Then << ":" << *Else << " )";
        }

        public:

        std::unique_ptr<AstNode> Condition;
        std::unique_ptr<AstNode> Then;
        std::unique_ptr<AstNode> Else;

        ~ConditionalNode() {}
        ConditionalNode(AstNode *condition, AstNode *then, AstNode *else_) : Condition(condition), Then(then), Else(else_) {}
        void CodeGen() {}
    };

    class AssignNode : public AstNode {

        void Print(std::ostream &out) const {
            out << "( " << *Left << " = " << *Right << " )";
        }

        public:

        std::unique_ptr<AstNode> Left;
        std::unique_ptr<AstNode> Right;

        ~AssignNode() {}
        AssignNode(AstNode *left, AstNode *right) : Left(left), Right(right) {}
        void CodeGen() {}
    };

    class BlockNode : public AstNode {

        void Print(std::ostream &out) const {
            out << "{ ";
            std::copy(Statements.begin(), Statements.end(), std::ostream_iterator<std::unique_ptr<AstNode>>(out));
            out << " }";
        }

        public:

        AstNodePtrs Statements;

        ~BlockNode() {}
        BlockNode(std::initializer_list<AstNode *> statements) {
//            for (auto const &s : statements) {
//                Statements.push_back(std::make_shared<AstNode>(s));
//            }
        }
        void CodeGen() {}
    };

}

#endif /*__SOTA_AST__*/
