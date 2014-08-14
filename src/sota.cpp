// sota.cpp : Defines the entry point for the console application.
//
#include <array>
#include <string>
#include <iostream>
#include <exception>

#include "z2h/parser.hpp"
#include "z2h/symbol.hpp"

#include "ast.h"
#include "parser.h"
#include "bindpower.h"

#include "tclap/CmdLine.h"

using namespace sota;

int main(int argc, char **argv) {
    try {
        TCLAP::CmdLine cmdline("Sota Description", ' ', "0.1");
        TCLAP::SwitchArg tokenize("t", "tokenize", "Tokenize the code", cmdline, false);
        TCLAP::ValueArg<std::string> code("c", "code", "Code to evaluate", true, "", "code");
        TCLAP::UnlabeledValueArg<std::string> file("file", "Sota file with main definined", true, "",  "file");
        cmdline.xorAdd(code, file);

        cmdline.parse(argc, argv);

        std::string source = code.getValue();
        std::string filename = file.getValue();

        auto parser = SotaParser();

        if (tokenize.getValue()) {
            std::vector<SotaToken *> tokens;
            if (!filename.empty()) {
                tokens = parser.TokenizeFile(filename);
            }
            else if (!source.empty()) {
                tokens = parser.Tokenize(source);
            }
            for (auto token : tokens)
                std::cout << *token << std::endl;
            return 0;
        }

        Ast *ast = nullptr;
        if (!filename.empty()) {
            ast = parser.ParseFile(filename);
            std::cout << filename << std::endl;
        }
        else if (!source.empty()) {
            ast = parser.Parse(source);
            std::cout << source << std::endl;
        }
        std::cout << ast->Print() << std::endl;
    }
    catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
    return 0;
}
