// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include "stdafx.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "stream.hpp"
#include <array>
#include <iostream>
#include <exception>

#include "ast.hpp"

using namespace sota;

class A {
    public:
    std::shared_ptr<int> M;
    A(int *m) : M(m) {}
};


int main(int argc, char* argv[]) {

    /*auto a1 = A(new int(1));
    auto a2 = A(new int(2));
    a1 = std::move(a2);*/

    try {
        Token add = { TokenType::Add, 0, 0 };
        auto id1 = new IdentifierNode("id1");
        auto id2 = new IdentifierNode("id2");
        auto infix = InfixOperatorNode(add, id1, id2);
        std::cout << infix << std::endl;

    }
    catch (const std::exception &ex) {
        std::cout << ex.what();
    }

    //auto dbln = [](double x) { return x * 2; };
    //std::cout << dbln(50) << std::endl;


    if (argc < 2)
        return 1;
    const char *filename = argv[1];
    auto parser = SotaParser();
    parser.ParseFile(filename);

    return 0;
}

