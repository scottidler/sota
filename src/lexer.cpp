#include "lexer.h"

namespace sota {
    using namespace stream;
    namespace lexer {

        SotaLexer::SotaLexer() {

        }
        SotaLexer::SotaLexer(string filename) : Line(1), Col(1) {
            Load(filename);
        }
        SotaLexer::SotaLexer(vector<char> chars) : Line(1), Col(1) {
            Load(chars);
        }

        void SotaLexer::Load(string filename) {
            ostringstream oss;
            ifstream fs(filename);
            oss << fs.rdbuf();
            const std::string str(oss.str());
            Load(vector<char>(str.begin(), str.end()));
        }

        void SotaLexer::Load(vector<char> chars) {
            chars.push_back('\0');
            _charstream = SotaStream<char>(chars);
        }

        Token SotaLexer::pop() {
            Token token = _tokens.top();
            _tokens.pop();
            return token;
        }

        Token SotaLexer::dots(char &c) {
            auto token = Token();
            if (c == '.') {
                if (_charstream.Peek('.')) {
                    token.value = "..";

                }
            }
            return token;
        }

        Token SotaLexer::regex(char &c) {
            auto token = Token();
            if (isoneof(c, "ms")) {
                if (isspace(_charstream.Prev()) && _charstream.Peek('/'))
                {
                    token.value += c;
                    token.value += _charstream.Next();
                    token.type = Value2Type[token.value];
                    c = _charstream.Next();
                }
            }
            return token;
        }

        Token SotaLexer::symbol(char &c) {
            auto index = _charstream.Index;
            auto token = Token();
            string s;
            if (startswith(s + c, TokenValues)) {
                s += c;
                while (startswith(s + _charstream.Peek(), TokenValues)) {
                    s += (c = _charstream.Next());
                }
            }
            else
                return token;
  
            if (Value2Type.count(s)) {
                do {
                    token.type = Value2Type[s];
                    token.value = s;
                    c = _charstream.Next();
                } while (Value2Type.count(s += c));
            }
            else
                c = _charstream.Curr(index);

            return token;
        }

        Token SotaLexer::numeral(char &c) {
            auto token = Token();
            return token;
        }

        Token SotaLexer::newline(char &c) {
            auto token = Token();
            if (isoneof(c,"\r\n")) {
                do {
                    ++Line;
                    c = _charstream.Next();
                } while (isoneof(c, "\r\n"));
                if (!c) {
                    token.type = TokenType::EndOfFile;
                    token.value = Type2Value[token.type];
                    _tokens.push(token);
                }
                token.type = TokenType::EndOfLine;
                token.value = Type2Value[token.type];
                return token;
            }
            return token;
        }

        Token SotaLexer::identifier(char &c) {
            auto token = Token();
            if (isalpha(c) || c == '_') {
                token.type = TokenType::Id;
                do {
                    ++Col;
                    token.value += c;
                    c = _charstream.Next();
                } while (isalnum(c) || c == '_');
            }
            return token;
        }

        Token SotaLexer::whitespace(char &c) {
            auto token = Token();
            if (isspace(c)) {
                while (isspace(c = _charstream.Next())) {
                    ++Col;
                }
            }
            return token;
        }

        Token SotaLexer::Scan() {
            Token token = Token();
            char c = _charstream.Curr();

            if (_tokens.size()) {
                return pop();
            }

            /* consume multiple newlines, emit only one token*/
            if (token = newline(c))
                return token;

            /* consume ws and return denting */
            if (token = whitespace(c))
                return token;

            /* match symbols and keywords */
            if (token = symbol(c))
                return token;

            /* regex operators m/ and s/ */
            if (token = regex(c))
                return token;

            /* match updir or range operators */
            if (token = dots(c))
                return token;

            /* id */
            if (token = identifier(c))
                return token;

            /* num */
            if (isdigit(c) || c == '.') {
                do {
                    ++Col;
                    token.value += c;
                } while (isdigit(c = _charstream.Next()));
                token.type = TokenType::Num;
                return token;
            }

            if (c == '#') {
                do {
                    ++Col;
                    token.value += c;
                    c = _charstream.Next();
                } while (c && c != '\n' && c != '\r');
                token.type = TokenType::Comment;
                token.value = Type2Value[token.type];
                return token;
            }

            if (!c) {
                token.type = TokenType::EndOfFile;
                token.value = Type2Value[token.type];
                return token;
            }

            return token;
        }
    }
}
