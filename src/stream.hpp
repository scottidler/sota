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
        protected:
            vector<Item> _items;
            static const Item default = Item();

            const Item&
            access(unsigned int index) {
                if (index >= _items.size())
                    return default;
                return _items[index];
            }
        public:
            unsigned int Index;
            Item Curr;

            ~SotaStream() { }
            SotaStream() : Index(0), Curr(default) { }
            SotaStream(vector<Item> items) : _items(items), Index(0), Curr(items[0]) { }

            virtual const Item&
            Prev(unsigned int lookback = 1) { return Curr = access(Index - lookback); }

            virtual const Item&
            Peek(unsigned int lookahead = 1) { return Curr = access(Index + lookahead); }

            virtual const Item&
            Next(unsigned int lookahead = 1) { return Curr = access(Index += lookahead); }

            virtual bool
            IsPrev(Item item, unsigned int lookback = 1) { return item == Prev(lookback); }

            virtual bool
            IsPeek(Item item, unsigned int lookahead = 1) { return item == Peek(lookahead); }

            virtual bool
            IsNext(Item item, unsigned int lookahead = 1) { return item == Next(lookahead); }

            virtual bool
            IsCurr(Item item) { return item == Curr; }
        };
    }
}

#endif /*__SOTA_STREAM__*/