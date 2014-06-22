#include "token.h"
#include "symbol.h"

namespace sota {

    Symbol::Symbol():
        Type(SymbolType::EndOfFile),
        Pattern(""),
        Scan(nullptr),
        Parse(nullptr),
        LBP(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, ParseFunc parse, size_t lbp):
        Type(type),
        Pattern(pattern),
        Scan(scan),
        Parse(parse),
        LBP(lbp) {
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
