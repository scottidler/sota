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

using namespace sota;

int main(int argc, char* argv[]) {
    if (argc < 2)
        return 1;
    const char *filename = argv[1];
    auto parser = SotaParser();
    parser.ParseFile(filename);

    return 0;
}

