// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include "stdafx.h"
#include "utils.h"
#include "lexer.h"
#include "token.h"
#include "stream.hpp"
#include <array>

using namespace sota;

std::vector<char> load(std::string filename) {
    std::vector<char> chars;
    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.eof() && !file.fail() ) {
        file.seekg(0, std::ios_base::end);
        auto size = (unsigned int)file.tellg();
        chars.resize(size);
        file.seekg(0, std::ios_base::beg);
        file.read(&chars[0], (long int)size);
    }
    return chars;
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        return 1;
    const char *filename = argv[1];
    std::cout << "sota parsing: " << filename << std::endl << std::endl;

    auto chars = load(filename);
    auto lexer = SotaLexer(chars);

    while(auto token = lexer.Scan()) {

        switch (token.Type) {
        case TokenType::EndOfLine:
            std::cout << lexer.Pretty(token) << std::endl;
            break;
        case TokenType::WhiteSpace:
            break;
        default:
            std::cout << lexer.Pretty(token) << " ";
            break;
        }

    }
    return 0;
}

