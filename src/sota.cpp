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
    std::cout << ast->Print() << std::endl;

    Token token = Token(nullptr, source, 0, 0);
    std::cout << token.Name() << std::endl;

    return 0;
}
