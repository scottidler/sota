// sota.cpp : Defines the entry point for the console application.
//
#include <array>
#include <string>
#include <iostream>
#include <exception>

#include "z2h/parser.hpp"
#include "z2h/symbol.hpp"

#include "ast.h"
#include "parser.h"
#include "bindpower.h"

/*
#define TEST(num) ((num % 2) ? (true) : (false))
#define IFNDEF(method) ((method != nullptr) ? (method) : (nullptr))

*/

int Add(int x, int y) {
    return x + y;
}
using namespace sota;

void usage() {
    std::cout << "usage" << std::endl;
}

std::vector<std::string> accumulate(int i, int argc, char *argv[]) {
    std::vector<std::string> items;
    for(; i < argc; ++i) {
        items.push_back(argv[i]);
    }
    return items;
}

int main(int argc, char* argv[]) {

    std::vector<std::string> filenames;
    std::vector<std::string> sources;
    switch (argc) {
        case 1:
            usage();
            return 0;
        default:
            if (std::string(argv[1]) == "-c") {
                sources = accumulate(2, argc, argv);
            }
            else {
                filenames = accumulate(1, argc, argv);
            }
            break;
    }

    auto parser = SotaParser();
    parser.source = sources[0];

    auto *token1 = parser.Consume();
    std::cout << "token: " << *token1 << std::endl;
    std::cout << "position: " << parser.position << std::endl;

    auto *token2 = parser.Consume();
    std::cout << "token: " << *token2 << std::endl;
    std::cout << "position: " << parser.position << std::endl;

    return 0;

    for (auto source : sources) {
        std::cout << "sources:" << std::endl;
        auto *ast = parser.Parse(source);
        std::cout << "tokens: " << *(parser.tokens[0]) << std::endl;
        std::cout << source << ":" << std::endl;
        std::cout << ast->Print() << std::endl;
    }

    for (auto filename : filenames) {
        std::cout << "filenames:" << std::endl;
        auto *ast = parser.ParseFile(filename);
        std::cout << filename << ":" << std::endl;
        std::cout << ast->Print() << std::endl;
    }
    return 0;
}
