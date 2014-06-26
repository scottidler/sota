// sota.cpp : Defines the entry point for the console application.
//
#include <array>
#include <string>
#include <iostream>
#include <exception>

#include "ast.h"
#include "token.h"
#include "parser.h"
#include "grammar.h"

using namespace sota;

int main(int argc, char* argv[]) {

    std::string source = "a + b";

    auto parser = Parser(Type2Symbol);
    auto *ast = parser.Parse(source);
    std::cout << "Parse result: " << ast->Print() << std::endl;

    return 0;
}
