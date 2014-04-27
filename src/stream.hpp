#ifndef __SOTA_STREAM__
#define __SOTA_STREAM__ = 1

#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

namespace sota {
    namespace stream {
        template <class Item>
        class SotaStream {
            vector<Item> _items;
        public:
            unsigned int Index;
            ~SotaStream() {
            }
            SotaStream() : Index(0) {

            }
            SotaStream(vector<Item> items) : _items(items), Index(0) {
            }

            const Item&
            Prev(unsigned int lookback = 1) { return _items[Index - lookback]; }

            bool
            Prev(Item item, unsigned int lookback = 1) { return item == Prev(lookback); }

            const Item&
            Peek(unsigned int lookahead = 1) { return _items[Index + lookahead]; }

            bool
            Peek(Item item, unsigned int lookahead = 1) { return item == Peek(lookahead); }

            const Item&
            Next(unsigned int lookahead = 1) { return _items[Index += lookahead]; }

            bool
            Next(Item item, unsigned int lookahead = 1) { return item == Next(lookahead); }

            const Item&
            Curr() { return _items[Index]; }

            bool
            Curr(Item item) { return item == Curr(); }

            const Item&
            Curr(unsigned int index) { return _items[Index = index]; }
        };
    }
}

#endif /*__SOTA_STREAM__*/