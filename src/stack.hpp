#ifndef __SOTA_STACK__
#define __SOTA_STACK__ = 1

#include "exceptions.h"

#include <stack>

namespace sota {

    /* Thin wrapper around built-in stack to provide some nicer default behavior
     *
     *
     *
     *
     *
     *
     */

    template <typename Type>
    struct SotaStack {

        std::stack<Type> items;

        ~SotaStack() {
        }
        SotaStack() {
        }

        size_t Size() const {
            return items.size();
        }

        void Push(Type item) {
            items.push(item);
        }

        Type Pop(Type item) {
            Type result = items.top();
            if (item && item != result)
                throw SotaException(__FILE__, __LINE__, "Pop: item expected");
            items.pop();
            return result;
        }

        Type Top() const {
            return items.empty() ? nullptr : items.top();
        }

        bool Top(Type item) const {
            return Top() == item;
        }
    };
}

#endif /*__SOTA_STACK__*/
