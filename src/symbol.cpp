#include "token.h"
#include "symbol.h"

namespace sota {

    Symbol::Symbol()
        : Type(SymbolType::EndOfFile)
        , Pattern("")
        , Scan(nullptr)
        , Nud(nullptr)
        , Led(nullptr)
        , LBP(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp)
        : Type(type)
        , Pattern(pattern)
        , Scan(scan)
        , Nud(nud)
        , Led(led)
        , LBP(lbp) {
    }

    Symbol::operator bool() {
        return Type != SymbolType::EndOfFile;
    }

    bool Symbol::operator==(const Symbol &rhs) {
        return Type == rhs.Type;
    }

    bool Symbol::operator!=(const Symbol &rhs) {
        return Type != rhs.Type;
    }
}
