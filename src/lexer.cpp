#include "lexer.h"

namespace sota {
    namespace lexer {

        Token SotaLexer::eol() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (_charstream.IsCurrAnyOf({ '\r', '\n' })) {
                token.Type = TokenType::EndOfLine;
                if (_charstream.IsCurrSeqOf({ '\r', '\n' }))
                    ++token.Count;
                _charstream.Next(token.Count);
            }
            return token;
        }

        Token SotaLexer::dent() {
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

        Token SotaLexer::ws() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (_charstream.IsCurrAnyOf({ ' ', '\t' })) {
                token.Type = TokenType::WhiteSpace;
                while (_charstream.IsNextAnyOf({ ' ', '\t' }))
                    ++token.Count;
            }
            return token;
        }

        Token SotaLexer::str() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            char c = _charstream.Curr;
            if ('\'' == c || '\"' == c) {
                if (_charstream.IsPeekSeqOf({ '-', '-', 'b', 'r' })) {
                    int x = 1;
                }
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

        Token SotaLexer::raw() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (!_charstream.IsCurrAnyOf({ ' ', '\t', '\r', '\n', '\0' })) {
                token.Type = TokenType::RAW;
                while (!_charstream.IsNextAnyOf({ ' ', '\t', '\r', '\n', '\0' })) {
                    ++token.Count;
                }
            }
            return token;
        }

        Token SotaLexer::eof() {
            Token token = { TokenType::EndOfFile, _charstream.Index, 1, &_charstream.Items };
            if (_charstream.IsCurr('\0')) {
                if (_tokens.back().Type != TokenType::EndOfLine)
                    _tokens.push_back({ TokenType::EndOfLine, _charstream.Index, 0, &_charstream.Items });
                while (_indents.top()) {
                    _tokens.push_back({ TokenType::Dedent, _charstream.Index, 0, &_charstream.Items });
                    _indents.pop();
                }
            }
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

            string s;
            for (int i = 460; i <= 470; ++i)
                s += _chars[i];
            cout << s << endl << endl;
            _tokens = vector<Token>();
            _tokenstream = SotaStream<Token>(_tokens);
        }

        vector<Token> SotaLexer::Pass1() {
            while (_charstream.Curr) {

                if (auto token = eol()) {
                    _tokens.push_back(token);
                    while (auto token = dent())
                        _tokens.push_back(token);
                    continue;
                }
                if (auto token = ws()) {
                    _tokens.push_back(token);
                    continue;
                }
                if (auto token = str()) {
                    _tokens.push_back(token);
                    continue;
                }
                if (auto token = raw()) {
                    _tokens.push_back(token);
                    continue;
                }
            }
            auto token = eof();
            _tokens.push_back(token);
            return _tokens;
        }

        vector<Token>
        SotaLexer::Pass2() {
            vector<Token> tokens;
            return tokens;
        }

        vector<Token>
        Tokenize() {
            vector<Token> tokens;
            return tokens;
        }
    }
}