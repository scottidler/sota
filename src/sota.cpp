// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include "stdafx.h"
#include "utils.h"
#include "lexer.h"
#include "token.h"
#include "stream.hpp"
#include <array>

using namespace std;
using namespace sota;
using namespace sota::lexer;
using namespace sota::stream;
using namespace sota::utils;

int main(int argc, char* argv[])
{
    string filename("example1.sota");
    auto lexer = SotaLexer(filename);

    Token t = { TokenType::Add, 1, 1 };
    TokenType tt = t;

    if (t == TokenType::Add)
        cout << "works" << endl;

    vector<string> values;
    Token token;
    do {
        token = lexer.Scan();
        values.push_back(token.Pretty());
        cout << token.Pretty() << " ";
        if (token.Type == TokenType::EndOfLine)
            cout << endl;
    } while (token);

    return 0;
}

