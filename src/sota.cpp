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

using namespace sota::stream;

int main(int argc, char* argv[])
{

    vector<char> chars = { 'a', 'b', 'c' };
    auto charstream = SotaStream<char>(chars);
    bool b = charstream.IsCurrSeqOf({ 'a', 'b' });

    string filename("example1.sota");
    auto lexer = SotaLexer(filename);

    auto tokens = lexer.Pass1();

    auto t2 = tokens[2].Value();

    vector<string> values;
    for (auto token : tokens) {
        auto value = Type2Value[token.Type];
        values.push_back(value == "RAW" || value == "STR" ? token.Value() : value);
    }

    return 0;
}

