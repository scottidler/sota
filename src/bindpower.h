#ifndef __SOTA_BINDPOWER__
#define __SOTA_BINDPOWER__ = 1

namespace sota {

    enum BindPower: size_t {
        None = 0,
        Assignment = 10,
        Question = 20,
        Logical = 30,
        Relational = 40,
        Sum = 50,
        Product = 60,
        Unary = 70,
        Dereference = 80,
    };
}

#endif /*__SOTA_BINDPOWER__*/
