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

    std::string source = "a * b + c";

    auto parser = Parser(Type2Symbol);
    parser.source = source;
    auto t1 = parser.Scan();
    std::cout << "expect a; t1=" << t1 << std::endl;
    auto t2 = parser.Consume();
    std::cout << "expect a; t2=" << t2 << std::endl;
    auto t3 = parser.LookAhead(1);
    std::cout << "expect *; t3=" << t3 << std::endl;
    auto t4 = parser.Consume();
    std::cout << "expect *; t4=" << t4 << std::endl;
    //auto *ast = parser.Parse(source);
    //std::cout << "Parse result: " << ast->Print() << std::endl;

    return 0;
}
