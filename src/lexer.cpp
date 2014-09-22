#include "z2h/token.hpp"
#include "z2h/lexer.hpp"

#include "lexer.h"
#include "exceptions.h"

#include "boost/regex.hpp"

namespace sota {

    SotaLexer::SotaLexer(std::string source)
        : z2h::Lexer(source) {
        #define T(t,p) {t,p},
        tokenmap = {
            TOKENS
        };
        #undef T
    }

    bool SotaLexer::IsRegex(std::string &pattern) {
        std::string prefix = "re:";
        if (pattern.size() >= prefix.size() && pattern.compare(0, prefix.size(), prefix) == 0) {
            pattern = pattern.substr(3, pattern.length() - 3);
            return true;
        }
        return false;
    }

    z2h::Token * SotaLexer::LiteralScan(size_t type, const std::string &pattern, const std::string &source, size_t position) {
        if (source.size() >= pattern.size() && source.compare(0, pattern.size(), pattern) == 0 )
            return new z2h::Token(type, pattern, position, pattern.size() );
        return nullptr;
    }

    z2h::Token * SotaLexer::RegexScan(size_t type, const std::string &pattern, const std::string &source, size_t position) {
        boost::smatch matches;
        const boost::regex re("(" + pattern + ")(?:\n|.)*");
        if (boost::regex_match(source, matches, re) ) {
            auto match = matches[1];
            return new z2h::Token(type, match, position, match.length() );
        }
        return nullptr;
    }

    z2h::Token * SotaLexer::Scan() {
        z2h::Token *match = nullptr;
        if (position < source.length() ) {
            auto text = source.substr(position, source.length() - position);
            for (auto iter = tokenmap.begin(); iter != tokenmap.end(); ++iter) {
                auto type = iter->first;
                auto pattern = iter->second;
                auto token = IsRegex(pattern) ? RegexScan(type, pattern, text, position) : LiteralScan(type, pattern, text, position);
                if (nullptr == match) {
                    match = token;
                }
                else if (nullptr != token && (token->length > match->length || (token->type < match->type && token->length == match->length) ) ) {
                    delete match;
                    match = token;
                }
                else {
                    delete token;
                }
            }
            if (nullptr == match) {
                std::cout << "Exception: nullptr == match=" << *match << std::endl;
                //throw std::exception("crap"); FIXME: throw an exception?
            }
            if (TokenType::WhiteSpace == match->type)
                match->skip = true;
            return match;
        }
        return new z2h::Token(TokenType::EndOfFile, "EOF", position, 0);
    }

    std::vector<z2h::Token *> SotaLexer::Tokenize() {
        std::vector<z2h::Token *> tokens;
        return tokens;
    }

}
