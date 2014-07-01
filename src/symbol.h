#ifndef __SOTA_SYMBOL__
#define __SOTA_SYMBOL__ = 1

#include <string>
#include <iostream>
#include <functional>


namespace sota {

    class Ast;
    class Token;
    class Parser;
    class Symbol;

    typedef std::function<long(Symbol *, const std::string &, size_t)> ScanFunc;
    typedef std::function<Ast *(Parser *, Token *)> NudFunc;
    typedef std::function<Ast *(Parser *, Ast *, Token *)> LedFunc;

    typedef struct Symbol {

        SymbolType      type;
        std::string     pattern;
        ScanFunc        Scan;
        NudFunc         Nud;
        LedFunc         Led;
        size_t          lbp;

        Symbol();
        Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp);

        operator bool();
        bool operator==(const Symbol &rhs);
        bool operator!=(const Symbol &rhs);

        friend std::ostream & operator<<(std::ostream &out, const Symbol &symbol);

    } Symbol;

}

#endif /*__SOTA_SYMBOL__*/
