#ifndef __SOTA_STREAM__
#define __SOTA_STREAM__ = 1

namespace sota {
    namespace stream {
        template <class Item>
        class SotaStream {
            const Item *_items;
            unsigned int _index;

            unsigned int _checked(unsigned int index) {
                if (0 <= index && index <= Size()) {
                    return index;
                }
                throw out_of_range("out of bounds in SotaStream _checked");
            }
        public:
            SotaStream(Item items[]): _items(items), _index(0) {

            }
            ~SotaStream() {

            }

            unsigned int Size() {
                return sizeof(_items) / sizeof(Item);
            }

            const Item & Prev(unsigned int lookback = 1) {
                return _items[_checked(_index - lookback)];
            }
            const Item& Peek(unsigned int lookahead = 1) {
                return _items[_checked(_index + lookahead)];
            }
            const Item& Next(unsigned int lookahead = 1) {
                return _items[_checked(_index += lookahead)];
            }
            const Item& Curr() {
                return _items[_checked(_index)];
            }
        };
    }
}

#endif /*__SOTA_STREAM__*/