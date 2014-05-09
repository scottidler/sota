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
    vector<string> values1 = {
        "sue",
        "sam",
        "sal",
        "ann",
    };

    string s1 = "bobby";
    auto b1 = startofany("su", values1);

    auto ks = keys(Value2Type);

    auto s = "12"[0];

    auto b = SymbolStart['n'];

    auto t = SymbolValue2Type["..."];

    string filename("example1.sota");
    auto lexer = SotaLexer(filename);

    vector<string> values;
    while (auto token = lexer.Scan()) {
        values.push_back(token.Pretty());
        cout << token.Pretty() << " ";
        if (token.Type == TokenType::EndOfLine)
            cout << endl;
    }

    return 0;
}

