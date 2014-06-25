// sota.cpp : Defines the entry point for the console application.
//
#include <boost/regex.hpp>
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

    std::string source = "1 + 2";
    boost::regex re("[0-9]+");
    boost::match_results<std::string::const_iterator> matches;
    boost::regex_match(source, matches, re);
    std::cout << matches[1].str() << std::endl;
    return 0;

    auto parser = Parser(Type2Symbol);
    parser.Source = source;
    auto token = parser.Scan();

    std::cout
        << token.Index << std::endl
        << token.Length << std::endl;

    return 0;
}
