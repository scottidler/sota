#ifndef __SOTA_BINDPOWER__
#define __SOTA_BINDPOWER__ = 1

namespace sota {

    enum BindPower: long {
        None = 0,
        Assignment = 10,
        Ternary = 20,
        Logical = 30,
        Comparison = 40,
        Sum = 50,
        Product = 60,
        Unary = 70,
        Dereference = 80,
        Regex = 90,
    };
}

#endif /*__SOTA_BINDPOWER__*/
