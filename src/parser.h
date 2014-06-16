#ifndef __SOTA_PARSER__
#define __SOTA_PARSER__ = 1

#include <queue>
#include <stack>
#include <string>
#include <vector>
#include "utils.h"
#include "token.h"
#include "stream.hpp"

namespace sota {
    class SotaParser : public SotaStream<Token> {

        std::vector<char> Load(std::string filename);

    public:
        ~SotaParser();
        SotaParser();

        void Parse(std::vector<char> source);
        void ParseFile(const char *filename);
    };
}

#endif /*__SOTA_PARSER__*/
