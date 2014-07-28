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

    for (auto source : sources) {
        std::cout << "sources:" << std::endl;
        /*
        parser.source = source;
        auto t0 = parser.LookAhead(1);
        std::cout << "t0=" << *t0 << std::endl;
        auto t1 = parser.LookAhead(2);
        std::cout << "t1=" << *t1 << std::endl;
        auto t2 = parser.LookAhead(3);
        std::cout << "t2=" << *t2 << std::endl;
        return 0;*/
        parser.source = source;
        for (auto token : parser.Tokenize())
            std::cout << *token << std::endl;
        return 0;
        auto *ast = parser.Parse(source);
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
