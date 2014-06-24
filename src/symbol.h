#ifndef __SOTA_SYMBOL__
#define __SOTA_SYMBOL__ = 1

#include <vector>
#include <iostream>
#include <functional>

namespace sota {

    typedef std::function<size_t(const std::string &, size_t)> ScanFunc;
    typedef std::function<Ast *(Parser *, Token *)> NudFunc;
    typedef std::function<Ast *(Parser *, Ast *, Token *)> LedFunc;

    class Ast;
    class Token;
    class Parser;

    typedef struct Symbol {

    public:

        SymbolType      Type;
        std::string     Pattern;
        ScanFunc        Scan;
        NudFunc         Nud;
        LedFunc         Led;
        size_t          LBP;

        Symbol();
        Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp);

        operator bool();
        bool operator==(const Symbol &rhs);
        bool operator!=(const Symbol &rhs);

        friend std::ostream & operator<<(std::ostream &out, const Symbol &symbol);
    } Struct;
}

#endif /*__SOTA_SYMBOL__*/
