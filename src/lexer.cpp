#include "lexer.h"
#include "token.h"
#include "utils.h"
#include "exceptions.h"

namespace sota {

    Token SotaLexer::Take(Token token) {
        if (token)
            _tokens.push_back(token);
        return token;
    }

    Token SotaLexer::Emit() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        if (_tokens.size()) {
        token = _tokens.front();
        _tokens.pop_front();
        }
        return token;
    }

    Token SotaLexer::EndOfLine() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        if (IsCurrAnyOf({ '\r', '\n' })) {
            ++token.Length;
            token.Type = TokenType::EndOfLine;
            if (IsCurrSeqOf({ '\r', '\n' }))
                ++token.Length;
            Next(token.Length);
        }
        while (Dent());
        return Take(token);
    }

    Token SotaLexer::Dent() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        if (IsPrevSeqOf({ '\r', '\n' }, 2) || IsPrevAnyOf({ '\r', '\n' })) {
            auto t = WhiteSpace();
            auto indent = t ? t.Length : 0;

            if (!_stride)
                _stride = indent;

            if (indent > _indents.top()) {
                if (indent == _indents.top() + _stride) {
                    _indents.push(indent);
                    token.Type = TokenType::Indent;
                }
                else
                    throw SotaException("indent didnt meet previously established stride of");
            }
            else if (indent < _indents.top()) {
                _indents.pop();
                token.Type = TokenType::Dedent;
                token.Length = 0;
            }
        }
        return Take(token);
    }

    Token SotaLexer::WhiteSpace() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        if (IsCurrAnyOf({ ' ', '\t' })) {
            ++token.Length;
            token.Type = TokenType::WhiteSpace;
            while (IsNextAnyOf({ ' ', '\t' }))
                ++token.Length;
        }
        return Take(token);
    }

    Token SotaLexer::Comment() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        if (IsCurr('#')) {
            ++token.Length;
            token.Type = TokenType::Comment;
            while (!IsNextAnyOf({ '\r', '\n' }))
                ++token.Length;
        }
        return Take(token);
    }

    Token SotaLexer::QuotedLiteral() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        char c = Curr;
        if ('\'' == c || '\"' == c) {
            ++token.Length;
            token.Type = TokenType::Str;
            while (!IsNextAnyOf({ c, '\r', '\n', '\0' }))
                ++token.Length;

            if (IsCurr(c)) {
                ++token.Length;
                Next();
            }
            else
                throw SotaException("missing end quote on string literal");
        }
        return Take(token);
    }

    Token SotaLexer::FlowLiteral() {
    	Token token = { TokenType::EndOfFile, Index, 0 };
        return Take(token);
    }

    Token SotaLexer::Literal() {
        Token token = QuotedLiteral();
        if (!token)
            token = FlowLiteral();
        return token;
    }

    Token SotaLexer::Symbol() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        std::string sym = "";
        auto symkeys = keys(SymbolValue2Type);

        while (startofany(sym + Curr, symkeys)) {
            sym += Curr;
            Next();
        }

        token.Type = SymbolValue2Type[sym];
        token.Length = Index - token.Index;

        if (!token.Type) {
            Index = token.Index; //backtrack
            token.Length = 0;
        }
        return Take(token);
    }

    Token SotaLexer::IdentifierNumberOrKeyword() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        bool loop = true;
        while (loop) {
            switch (Curr) {
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
            ++token.Length;
            Next();

        }
        auto value = Value(token);
        if (KeywordValue2Type.count(value))
            token.Type = KeywordValue2Type[value];
        return Take(token);
    }

    Token SotaLexer::EndOfFile() {
        Token token = { TokenType::EndOfFile, Index, 0 };
        return token;
    }

    /* public */

    SotaLexer::~SotaLexer() {

    }

    SotaLexer::SotaLexer(std::vector<char> chars) : SotaStream(chars) {
        _stride = 0;
        _indents.push(0);
    }

    unsigned int SotaLexer::Line(const Token &token) {
        /*
        regex const pattern("\r?\n");
        string head = string(Items.begin(), Items.begin() + token.Index);
        ptrdiff_t const count(distance(
            sregex_iterator(head.begin(), head.end(), pattern),
            sregex_iterator()));
        return count + 1;
        */
        return 0;
    }

    unsigned int SotaLexer::Column(const Token &token) {
        unsigned int count = token.Index;
        for (; count > 0; --count) {
            char c = Items[count];
            if ('\n' == c || '\r' == c) {
                break;
            }
        }
        return token.Index - count;
    }

    std::string SotaLexer::Value(const Token &token) {
        return std::string(Items.begin() + token.Index, Items.begin() + token.Index + token.Length);
    }

    std::string SotaLexer::Pretty(const Token &token) {
        std::string result;
        auto value = Value(token);
        auto tokenvalue = Type2Value[token.Type];
        switch (token.Type) {
        case TokenType::WhiteSpace:
        case TokenType::EndOfFile:
        case TokenType::EndOfLine:
        case TokenType::Indent:
        case TokenType::Dedent:
            result = tokenvalue;
            break;
        default:
            result = value != tokenvalue ? tokenvalue + ":" + value : value;
            break;
        }
        return result;
    }

    Token SotaLexer::Scan() {

        //Length = 0;

        if (_tokens.size()) {
            auto token = _tokens.front();
            _tokens.pop_front();
            return token;
        }
        if (auto token = Emit())
            return token;

        if (auto token = EndOfLine())
            return token;

        if (auto token = WhiteSpace())
            return token;

        if (auto token = Comment())
            return token;

        if (auto token = Literal())
            return token;

        if (auto token = Symbol())
            return token;

        if (auto token = IdentifierNumberOrKeyword())
            return token;

        return EndOfFile();
    }

    std::vector<Token> SotaLexer::Tokenize() {
        auto tokens = std::vector<Token>();
        return tokens;
    }
}
