#include "token.h"

namespace sota {

    /*Symbol*/

    Symbol::Symbol()
        : type(SymbolType::EndOfFile)
        , pattern("")
        , Scan(nullptr)
        , Nud(nullptr)
        , Led(nullptr)
        , lbp(0) {
    }
    Symbol::Symbol(SymbolType type, std::string pattern, ScanFunc scan, NudFunc nud, LedFunc led, size_t lbp)
        : type(type)
        , pattern(pattern)
        , Scan(scan)
        , Nud(nud)
        , Led(led)
        , lbp(lbp) {
    }

    Symbol::operator bool() {
        return type != SymbolType::EndOfFile;
    }

    bool Symbol::operator==(const Symbol &rhs) {
        return type == rhs.type;
    }

    bool Symbol::operator!=(const Symbol &rhs) {
        return type != rhs.type;
    }

    std::ostream & operator<<(std::ostream &out, const Symbol &symbol) {
        return out << "Symbol(type=" << symbol.type << ", pattern=" << symbol.pattern << ", lbp=" << symbol.lbp <<  ")";
    }

}
