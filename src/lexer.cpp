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

        const char&
        SotaLexer::Next(unsigned int lookahead) {
            if ('\r' == Curr || '\n' == Curr) {
                ++Line;
                Col = 1;
            }
            else
                ++Col;
            return SotaStream::Next(lookahead);
        }

        void 
        SotaLexer::Load(string filename) {
            ostringstream oss;
            ifstream fs(filename);
            oss << fs.rdbuf();
            const std::string str(oss.str());
            Load(vector<char>(str.begin(), str.end()));
        }

        void 
        SotaLexer::Load(vector<char> chars) {
            _items = chars;
            Curr = _items[0];
        }

        Token 
        SotaLexer::pop() {
            Token token = _tokens.top();
            _tokens.pop();
            return token;
        }

        Token 
        SotaLexer::dots(char &c) {
            auto token = Token();
            if (c == '.') {
                if (Peek('.')) {
                    token.value = "..";
                }
            }
            return token;
        }

        Token 
        SotaLexer::regex(char &c) {
            auto token = Token();
            if (isoneof(c, "ms")) {
                if (isspace(Prev()) && Peek('/'))
                {
                    token.value += c;
                    token.value += Next();
                    token.type = Value2Type[token.value];
                    c = Next();
                }
            }
            return token;
        }

        Token 
        SotaLexer::comment(char &c) {
            auto token = Token();
            if (c == '#') {
                do {
                    ++Col;
                    token.value += c;
                    c = Next();
                } while (c && c != '\n' && c != '\r');
            }
            return token;
        }

        Token 
        SotaLexer::newline(char &c) {
            auto token = Token();
            if (isoneof(c,"\r\n")) {
                do {
                    ++Line;
                    c = Next();
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

        Token 
        SotaLexer::whitespace(char &c) {
            auto token = Token();
            if (isspace(c)) {
                while (isspace(c = Next())) {
                    ++Col;
                }
            }
            return token;
        }

        Token 
        SotaLexer::identifier_numeral_or_symbol(char &c) {
            auto token = Token();
            do {
                switch(c) {
                case 'a':   case 'A':
                case 'b':   case 'B':
                case 'c':   case 'C':
                case 'd':   case 'D':
                case 'e':   case 'E':
                case 'f':   case 'F':
                case 'g':   case 'G':
                case 'h':   case 'H':
                case 'i':   case 'I':
                case 'j':   case 'J':
                case 'k':   case 'K':
                case 'l':   case 'L':
                case 'm':   case 'M':
                case 'n':   case 'N':
                case 'o':   case 'O':
                case 'p':   case 'P':
                case 'q':   case 'Q':
                case 'r':   case 'R':
                case 's':   case 'S':
                case 't':   case 'T':
                case 'u':   case 'U':
                case 'v':   case 'V':
                case 'w':   case 'W':
                case 'x':   case 'X':
                case 'y':   case 'Y':
                case 'z':   case 'Z':
                case '_':   
                    token.type = TokenType::Id;
                    token.value += c;
                    c = Next();
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (token.type != TokenType::Id)
                        token.type = TokenType::Num;
                    token.value += c;
                    c = Next();
                    break;
                default:
                    if (c && !token) {
                        while (startswith(token.value + c, TokenValues)) {
                            token.value += c;
                            c = Next();
                        }
                        if (Value2Type.count(token.value)) {
                            token.type = Value2Type[token.value];
                            return token;
                        }
                        else
                            throw exception("match failure in identifier_numeral_or_symbol");
                    }
                    break;
                }
            } while (c && isalnum(c) || c == '_');
            if (Value2Type.count(token.value))
                token.type = Value2Type[token.value];
            return token;
        }

        Token 
        SotaLexer::Scan() {
            Token token = Token();
            char c = Curr;

            if (_tokens.size()) {
                return pop();
            }

            /* consume multiple newlines, emit only one token*/
            if (token = newline(c))
                return token;

            /* consume ws and return denting */
            if (token = whitespace(c))
                return token;

            /* remove comments */
            if (token = comment(c))
                return token;

            /* regex operators m/ and s/ */
            if (token = regex(c))
                return token;

            /* match updir or range operators */
            if (token = dots(c))
                return token;

            /* divine id, num or keyword */
            if (token = identifier_numeral_or_symbol(c))
                return token;
            
            if (!c) {
                token.type = TokenType::EndOfFile;
                token.value = Type2Value[token.type];
                return token;
            }

            return token;
        }
    }
}
