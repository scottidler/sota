#include "parser.h"
#include "lexer.h"
#include <fstream>

namespace sota {

    std::vector<char> SotaParser::Load(std::string filename) {
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

    SotaParser::~SotaParser() {}
    SotaParser::SotaParser() {}
        
    void SotaParser::Parse(std::vector<char> source) {
        auto lexer = SotaLexer(source);
        
        Token token;
        while(token = lexer.Scan()) {

            switch (token.Type) {
            case TokenType::EndOfLine:
                std::cout << lexer.Pretty(token) << std::endl;
                break;
            case TokenType::WhiteSpace:
                break;
            default:
                std::cout << lexer.Pretty(token) << " ";
                break;
            }
        }
        std::cout << lexer.Pretty(token) << std::endl;
    }
    void SotaParser::ParseFile(const char *filename) {
        auto source = Load(filename);
        Parse(source);
    }
}
