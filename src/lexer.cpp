#include "lexer.h"
#include "utils.h"

namespace sota {
    using namespace utils;
    namespace lexer {
        char SotaLexer::prevchar(int lookback) {
            if (_index - lookback >= 0)
                return _code[_index - lookback];
            return '\0';
        }
        char SotaLexer::peekchar(int lookahead) {
            if (_index + lookahead <= (int)_code.length())
                return _code[_index + lookahead];
            return EOF;
        }
        char SotaLexer::nextchar() {
            char c = peekchar();
            ++_col;
            if (prevchar() == '\n') {
                ++_line;
                _col = -1;
            }
            ++_index;
            return c;
        }
        bool SotaLexer::ischar(char c, bool advance) {
            bool match = prevchar() == c;
            if (match && advance)
                nextchar();
            return match;
        }
        bool SotaLexer::inrange(char c1, char c2, bool advance) {
            char c = prevchar();
            bool match = (c1 <= c && c <= c2);
            if (match && advance)
                nextchar();
            return match;
        }
        SotaLexer::SotaLexer(string filename) {
            Load(filename);
        }
        SotaLexer::~SotaLexer() {
        }
        void SotaLexer::Load(string filename) {
            std::ifstream in(filename);
            _code = string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            _col = -1;
            _line = 0;
            _index = -1;
            _tokens = vector<lexer::Token>();
            
            char c;
            while (c = nextchar()) {

                if (_col == 0) {
                    _tokens.push_back(Token(_line, _col, TokenType::META_BOL));
                }

                while (c != '\n' && isspace(c))
                    c = nextchar();

                
                if (isalpha(c)) {
                    string id;
                    id += c;
                    while (isalnum(c = nextchar())) {
                        id += c;
                    }

                    if (TokenTypes.count(id)) {
                        _tokens.push_back(Token(_line, _col, TokenTypes[id], id));
                        continue;
                    }
                    else
                        _tokens.push_back(Token(_line, _col, TokenType::META_ID, id));
                }

                if (isdigit(c) || c == '.') {
                    string num;
                    do {
                        num += c;
                        c = nextchar();
                    } while (isdigit(c) || c == '.');
                    _tokens.push_back(Token(_line, _col, TokenType::META_NUM, num));
                }

                if (c == '#') {
                    do c = nextchar();
                    while (c != 0 && c != '\n' && c != '\r');
                }

                if (c == '\n') {
                    _tokens.push_back(Token(_line, _col, TokenType::META_EOL));
                    _col = 0;
                    ++_line;
                    continue;
                }

                if (c == EOF) {
                    _tokens.push_back(Token(_line, _col, TokenType::META_EOL));
                    _tokens.push_back(Token(_line, _col, TokenType::META_EOS));
                    _tokens.push_back(Token(_line, _col, TokenType::META_EOF));
                }
                string s;
                s += c;
                if (TokenTypes.count(s)) {
                    _tokens.push_back(Token(_line, _col, TokenTypes[s]));
                }
            }

        }
        Token SotaLexer::Prev(int lookback) {
            return Token();
        }
        Token SotaLexer::Peek(int lookahead) {
            return Token();
        }
        Token SotaLexer::Next() {
            return Token();
        }
    }
}