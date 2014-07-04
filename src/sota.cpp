// sota.cpp : Defines the entry point for the console application.
//
#include <array>
#include <string>
#include <iostream>
#include <exception>

#include "ast.hpp"
#include "z2h/parser.hpp"
#include "grammar.hpp"

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

    auto grammar = new SotaGrammar();
    auto parser = z2h::Parser<Ast *>(grammar);

    for (auto source : sources) {
        auto *ast = parser.Parse(source);
        std::cout << source << ":" << std::endl;
        std::cout << ast->Print() << std::endl;
    }

    for (auto filename : filenames) {
        auto *ast = parser.ParseFile(filename);
        std::cout << filename << ":" << std::endl;
        std::cout << ast->Print() << std::endl;
    }
    return 0;
}
