#ifndef __SOTA_GRAMMAR__
#define __SOTA_GRAMMAR__ = 1

#include "z2h/symbol.hpp"
#include "z2h/grammar.h"

namespace sota {

    class Ast;
    class SotaGrammar : public z2h::Grammar<Ast *> {
    
        std::vector<z2h::Symbol<Ast *> *> Symbols() {
            std::vector<z2h::Symbol<Ast *> *> symbols;
            return symbols;
        }
    };

}

#endif /*__SOTA_GRAMMAR__*/
