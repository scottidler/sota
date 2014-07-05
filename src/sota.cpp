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


    //Symbol(long type, long lbp, std::string pattern, ScanFunc<TAst> scan, StdFunc<TAst> std, NudFunc<TAst> nud, LedFunc<TAst> led)
    //auto symbol = new z2h::Symbol<Ast *>(SotaGrammar::SymbolType::Add, BindPower::Sum, "+", nullptr, nullptr, nullptr, nullptr);

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
    std::cout << 1 << std::endl;
    auto symbols = parser.Symbols();
    std::cout << symbols.size() << std::endl;
    auto symbol = symbols[0];
    std::cout << 3 << std::endl;

    std::cout << symbol << std::endl;

    return 0;

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
