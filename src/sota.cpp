// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include "stdafx.h"
#include "lexer.h"
#include "token.h"
#include "stream.hpp"
#include <array>

using namespace std;
using namespace sota;
using namespace sota::lexer;

int main(int argc, char* argv[])
{

    string str = "token1 token2 token3";
    string sub(str.substr(7, 6));

    string filename = "example1.sota";
    auto lexer = SotaLexer(filename);

    auto token = lexer.Scan();
    do {
        cout << token;
        token.type == TokenType::EndOfLine ? cout << endl : cout << " ";

        token = lexer.Scan();
    } while (token.type);
    cout << token << endl;

    return 0;
}

