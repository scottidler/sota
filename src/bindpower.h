#ifndef __SOTA_BINDPOWER__
#define __SOTA_BINDPOWER__ = 1

namespace sota {

    enum BindPower: long {
        None = 0,
        Denting = 10,
        Assignment = 10,
        Separator = 15,
        Ternary = 20,
        Logical = 30,
        Comparison = 40,
        Sum = 50,
        Product = 60,
        Unary = 70,
        Group = 75,
        Dereference = 80,
        Regex = 90,
        Define = 100,
        Kvp = 110,
    };
}

#endif /*__SOTA_BINDPOWER__*/
