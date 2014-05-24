// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include "stdafx.h"
#include "utils.h"
#include "lexer.h"
#include "token.h"
#include "stream.hpp"
#include "ast.h"
#include <array>

using namespace sota;

vector<char> load(string filename) {
    vector<char> chars;
    ifstream file(filename, ios::in | ios::binary | ios::ate);
    if (!file.eof() && !file.fail()) {
        file.seekg(0, ios_base::end);
        auto size = (unsigned int)file.tellg();
        chars.resize(size);
        file.seekg(0, ios_base::beg);
        file.read(&chars[0], size);
    }
    return chars;
}

int main(int argc, char* argv[])
{
    auto chars = load("example1.sota");
    auto lexer = SotaLexer(chars);

    vector<string> values;
    vector<string> pretties;
    vector<Token> tokens;
    while (auto token = lexer.Scan()) {

        cout << lexer.Pretty(token);
        if (TokenType::EndOfLine == token.Type)
            cout << endl;
        else
            cout << " ";
    }
    return 0;
}

