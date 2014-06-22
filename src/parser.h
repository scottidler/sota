#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <string>

#include "ast.h"

namespace sota {

    class Ast;
    class Parser {

        public:
        Ast * Parse(const std::string &source) {
            return nullptr;
        }
        Ast * ParseFile(const std::string &filename) {
            return Parse("");
        }
    };
}

#endif /*__SOTA_PARSER__*/
