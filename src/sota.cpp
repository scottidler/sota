// sota.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lexer.h"

#include <regex>

using namespace std;
using namespace sota;

int main(int argc, char* argv[])
{
    char c = EOF;
    string result;
    string text1 = "scott";
    string text2 = "m0rdred13";
    string text3 = "3times";
    smatch m;
    regex re("([0-9]+)?[A-Za-z_]\\w*");

    while (regex_search(text1, m, re)) {
        for (auto x : m)
            cout << x << " ";
        cout << endl;
        result = m.suffix().str();
    }

    auto token = lexer::Token();
    auto lexer = lexer::SotaLexer("example1.sota");
    return 0;
}

