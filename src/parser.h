#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <string>

#include "ast.h"
#include "token.h"

namespace sota {

    class Ast;
    class Parser {

        std::string _source;

        Token Consume();

        public:

        Parser();
        Parser(const std::string &source);

        Ast * ParseFile(const std::string &filename);
        Ast * Parse(const std::string &source);
        Ast * Parse(size_t lbp = 0);
    };
}

#endif /*__SOTA_PARSER__*/
