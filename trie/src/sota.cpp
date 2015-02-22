// sota.cpp : Defines the entry point for the console application.
//
#include <array>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <exception>
#include <sys/stat.h>

#include <string>

#include "trie.hpp"

#include "tclap/CmdLine.h"

std::string Open(const std::string &filename) {
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0)
        std::cout  << "Open: filename=" << filename << std::endl;
        //Exception(__FILE__, __LINE__, filename + " doesn't exist or is unreadable"); FIXME: this need to be addressed
    std::ifstream file(filename);
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return source;
}

int main(int argc, char **argv) {
    try {

        auto trie = new sota::SotaTrie<std::string, int>();
        TCLAP::CmdLine cmdline("Sota Description", ' ', "0.1");
        TCLAP::SwitchArg tokenize("t", "tokenize", "Tokenize the code", cmdline, false);
        TCLAP::ValueArg<std::string> code("c", "code", "Code to evaluate", true, "", "code");
        TCLAP::UnlabeledValueArg<std::string> file("file", "Sota file with main definined", true, "",  "file");
        cmdline.xorAdd(code, file);

        cmdline.parse(argc, argv);
        std::string source = code.getValue();
        std::string filename = file.getValue();

        //sota::Hasher<std::string, int> hasher;
        //auto trie = new sota::SotaTrie<std::string, int, sota::Hasher<std::string, int>>(hasher);
        int a = 40;
        trie->Set("scott", a);
        int *age = trie->Get("scott");
        age = age;

        if (filename.size() ) {
            source = Open(filename);
        }
        /*
        auto lexer = new SotaLexer(source);
        auto parser = new SotaParser(lexer);

        if (tokenize.getValue()) {
            auto tokens = parser->Tokenize();
            for (auto token : tokens)
                std::cout << *token << std::endl;
            return 0;
        }

        std::cout << "source: " << source << std::endl;
        auto ast = parser->Parse();
        std::cout << *ast << std::endl;

        delete lexer;
        delete parser;
        */
    }
    catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
    catch (std::exception const &e) {
        std::cerr << "exception: " << e.what() << std::endl;
        printf("ex: %s", e.what() );
    }
    return 0;
}
