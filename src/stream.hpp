#ifndef __SOTA_STREAM__
#define __SOTA_STREAM__ = 1

#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <initializer_list>

namespace sota {
    template <class Item>
    class SotaStream {

        const Item &
        access(unsigned int index) {
            if (index >= Items.size())
                return Default;
            return Items[index];
        }
    public:
        static const Item Default;

        std::vector<Item> Items;
        unsigned int Index;
        Item Curr;

        virtual ~SotaStream() { }
        SotaStream() : Index(0), Curr(Default) { }
        SotaStream(std::vector<Item> &items) : Items(items), Index(0), Curr(items.size() ? items[0] : Default) {

        }
            
        virtual const Item &
        Prev(unsigned int lookback = 1) { return access(Index - lookback); }

        virtual const Item &
        Peek(unsigned int lookahead = 1) { return access(Index + lookahead); }

        virtual const Item &
        Next(unsigned int lookahead = 1) { return Curr = access(Index += lookahead); }

        virtual bool 
        IsPrev(Item item, unsigned int lookback = 1) { return item == Prev(lookback); }

        virtual bool
        IsPeek(Item item, unsigned int lookahead = 1) { return item == Peek(lookahead); }

        virtual bool
        IsNext(Item item, unsigned int lookahead = 1) { return item == Next(lookahead); }

        virtual bool
        IsCurr(Item item) { return item == Curr; }

        virtual bool
        IsPrevAnyOf(std::initializer_list<Item> items, unsigned int lookback = 1) {
            auto prev = Prev(lookback);
            for (auto item : items) {
                if (item == prev)
                    return true;
            }
            return false;
        }

        virtual bool
        IsPeekAnyOf(std::initializer_list<Item> items, unsigned int lookahead = 1) {
            auto peek = Peek(lookahead);
            for (auto item : items) {
                if (item == peek)
                    return true;
            }
            return false;
        }

        virtual bool
        IsNextAnyOf(std::initializer_list<Item> items, unsigned int lookahead = 1) {
            auto next = Next(lookahead);
            for (auto item : items) {
                if (item == next)
                    return true;
            }
            return false;
        }

        virtual bool 
        IsCurrAnyOf(std::initializer_list<Item> items) {
            for (auto item : items) {
                if (item == Curr)
                    return true;
            }
            return false;
        }

        virtual bool 
        IsPrevSeqOf(std::initializer_list<Item> items, unsigned int lookback = 1) {
            unsigned int i = 0;
            for (auto item : items) {
                if (item != (Item)Peek(i++ - lookback))
                    return false;
            }
            return true;
        }

        virtual bool 
        IsPeekSeqOf(std::initializer_list<Item> items, unsigned int lookahead = 1) {
            unsigned int i = 0;
            for (auto item : items) {
                if (item != (Item)Peek(lookahead + i++))
                    return false;
            }
            return true;
        }

        virtual bool 
        IsNextASeqOf(std::initializer_list<Item> items, unsigned int lookahead = 1) {
            bool result = true;
            for (auto item : items) {
                if (item != (Item)Next() && result)
                    result = false;
            }
            return result;
        }

        virtual bool 
        IsCurrSeqOf(std::initializer_list<Item> items) {
            unsigned int i = 0;
            for (auto item : items) {
                if (item != Peek(i++))
                    return false;
            }
            return true;
        }
    };

    template<class Item>
    const Item SotaStream<Item>::Default = Item();
}

#endif /*__SOTA_STREAM__*/
