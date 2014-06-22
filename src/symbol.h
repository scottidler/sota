#ifndef __SOTA_SYMBOL__
#define __SOTA_SYMBOL__ = 1

namespace sota {

    class Ast;
    class Token;
    class Parser;
    class Symbol {

        public:
        typedef std::function<size_t(const std::string &, size_t)> ScanFunc;
        typedef std::function<Ast*(Parser*, Ast*, Token)> ParseFunc;

        SymbolType      Type;
        std::string     Pattern;
        ScanFunc        Scan;
        ParseFunc       Parse;
        size_t          LBP;

        Symbol();
        Symbol(SymbolType type, std::string pattern, ScanFunc scan, ParseFunc parse, size_t lbp);

        operator bool();
        bool operator==(const Symbol &rhs);
        bool operator!=(const Symbol &rhs);
    };
}

#endif
