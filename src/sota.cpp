// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include <array>
#include <iostream>
#include <exception>

#include "ast.h"
#include "token.h"
#include "parser.h"
using namespace sota;

int main(int argc, char* argv[]) {

    std::string source = "1 + 2";
    auto parser = Parser();
    Ast *ast = parser.Parse(source);
    //std::cout << ast->Print() << std::endl;

    auto symbol = Type2Symbol[SymbolType::Add];
    Token token = Token(&symbol, source, 2, 1);
    std::cout << token << std::endl;

    for(auto &item : Type2Symbol) {
        std::cout << item.first << item.second << std::endl;
    }
    return 0;
}
