#include "lexer.h"
#include "utils.h"

namespace sota {
    namespace lexer {

        Token 
        SotaLexer::eol() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (_charstream.IsCurrAnyOf({ '\r', '\n' })) {
                token.Type = TokenType::EndOfLine;
                if (_charstream.IsCurrSeqOf({ '\r', '\n' }))
                    ++token.Count;
                _charstream.Next(token.Count);
            }
            while (auto token = dent())
                _tokens.push_back(token);
            return token;
        }

        Token 
        SotaLexer::dent() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };

            if (_charstream.IsPrevSeqOf({ '\r', '\n' }, 2) || _charstream.IsPrevAnyOf({ '\r', '\n' })) {
                auto t = ws();
                auto indent = t ? t.Count : 0;

                if (!_stride)
                    _stride = indent;

                if (indent > _indents.top()) {
                    if (indent == _indents.top() + _stride) {
                        _indents.push(indent);
                        token.Type = TokenType::Indent;
                        token.Count = _stride;
                    }
                    else
                        throw exception("indent didnt meet previously established stride of %d", _stride);
                }
                else if (indent < _indents.top()) {
                    _indents.pop();
                    token.Type = TokenType::Dedent;
                    token.Count = 0;
                }
            }
            return token;
        }

        Token 
        SotaLexer::ws() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (_charstream.IsCurrAnyOf({ ' ', '\t' })) {
                token.Type = TokenType::WhiteSpace;
                while (_charstream.IsNextAnyOf({ ' ', '\t' }))
                    ++token.Count;
            }
            return token;
        }

        Token 
        SotaLexer::comment() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (_charstream.IsCurr('#')) {
                token.Type = TokenType::Comment;
                while (!_charstream.IsNextAnyOf({ '\r', '\n' }))
                    ++token.Count;
            }
            return token;
        }

        Token 
        SotaLexer::lit() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            char c = _charstream.Curr;
            if ('\'' == c || '\"' == c) {
                token.Type = TokenType::Str;
                while (!_charstream.IsNextAnyOf({ c, '\r', '\n', '\0' }))
                    ++token.Count;

                if (_charstream.IsCurr(c)) {
                    ++token.Count;
                    _charstream.Next();
                }
                else
                    throw exception("missing end quote on string literal");
            }

            return token;
        }

        Token 
        SotaLexer::sym() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 0, &_charstream.Items };

            string sym = "";
            auto symkeys = keys(SymbolValue2Type);

            while (startofany(sym + _charstream.Curr, symkeys)) {
                sym += _charstream.Curr;
                ++token.Count;
                _charstream.Next();
            }

            token.Type = SymbolValue2Type[sym];

            if (!token)
                _charstream.Index = token.Index; //backtrack

            return token;
        }

        Token 
        SotaLexer::id_num_kw() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 0, &_charstream.Items };

            bool loop = true;
            while (loop) {
                switch (_charstream.Curr) {
                case 'a':   case 'n':   case 'A':   case 'N':
                case 'b':   case 'o':   case 'B':   case 'O':
                case 'c':   case 'p':   case 'C':   case 'P':
                case 'd':   case 'q':   case 'D':   case 'Q':
                case 'e':   case 'r':   case 'E':   case 'R':
                case 'f':   case 's':   case 'F':   case 'S':
                case 'g':   case 't':   case 'G':   case 'T':
                case 'h':   case 'u':   case 'H':   case 'U':
                case 'i':   case 'v':   case 'I':   case 'V':
                case 'j':   case 'w':   case 'J':   case 'W':
                case 'k':   case 'x':   case 'K':   case 'X':
                case 'l':   case 'y':   case 'L':   case 'Y':
                case 'm':   case 'z':   case 'M':   case 'Z':
                case '_':
                    token.Type = TokenType::Id;
                    break;

                case '0':   case '5':
                case '1':   case '6':
                case '2':   case '7':
                case '3':   case '8':
                case '4':   case '9':
                    if (token.Type != TokenType::Id)
                        token.Type = TokenType::Num;
                    break;

                default:
                    loop = false;
                    continue;
                }
                ++token.Count;
                _charstream.Next();

            }
            if (KeywordValue2Type.count(token.Value()))
                token.Type = KeywordValue2Type[token.Value()];

            return token;
        }

        Token 
        SotaLexer::eof() {
            Token token = { TokenType::EndOfLine, _charstream.Index, 0, &_charstream.Items };
            while (_indents.top()) {
                _tokens.push_back({ TokenType::Dedent, _charstream.Index, 0, &_charstream.Items });
                _indents.pop();
            }
            _tokens.push_back({});
            return token;
        }

        /* public */
        SotaLexer::~SotaLexer() { 
        
        }

        SotaLexer::SotaLexer(string filename) {
            Load(filename);
        }

        void
        SotaLexer::Load(string filename) {
            _stride = 0;
            _indents = stack<unsigned int>();
            _indents.push(0);
            ifstream file(_filename = filename, ios::in | ios::binary | ios::ate);
            if (!file.eof() && !file.fail()) {
                file.seekg(0, ios_base::end);
                unsigned int size = (unsigned int)file.tellg();
                _chars = vector<char>();
                _chars.resize(size);
                file.seekg(0, ios_base::beg);
                file.read(&_chars[0], size);
            }
            _charstream = SotaStream<char>(_chars);
            _tokens = deque<Token>();
        }

        Token 
        SotaLexer::Scan() {

            if (_tokens.size()) {
                auto token = _tokens.front();
                _tokens.pop_front();
                return token;
            }

            if (auto token = eol())
                return token;

            if (auto token = ws())
                return token;

            if (auto token = comment())
                return token;

            if (auto token = lit())
                return token;

            if (auto token = sym())
                return token;

            if (auto token = id_num_kw())
                return token;

            return eof();
        }

        vector<Token>
        SotaLexer::Tokenize() {
            vector<Token> tokens;
            Token token;
            do {
                token = Scan();
                tokens.push_back(token);
            } while (token);
            return tokens;
        }
    }
}