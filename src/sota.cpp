// sota.cpp : Defines the entry point for the console application.
//
#include <regex>
#include "stdafx.h"
#include "utils.h"
#include "lexer.h"
#include "token.h"
#include "stream.hpp"
#include <array>

using namespace sota;

std::vector<char> load(std::string filename) {
	std::vector<char> chars;
	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.eof() && !file.fail() ) {
		file.seekg(0, std::ios_base::end);
		auto size = (unsigned int)file.tellg();
		chars.resize(size);
		file.seekg(0, std::ios_base::beg);
		file.read(&chars[0], (long int)size);
	}
	return chars;
}

int main(int argc, char* argv[])
{
	auto chars = load("example1.sota");
	auto lexer = SotaLexer(chars);

    while(auto token = lexer.Scan()) {
    	std::cout << lexer.Pretty(token);
    	if (TokenType::EndOfLine == token.Type)
    		std::cout << std::endl;
    	else
    		std::cout << " ";
    }
    return 0;
}

