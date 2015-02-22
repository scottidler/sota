#ifndef __SOTA_TRIE__
#define __SOTA_TRIE__ = 1

#define popcount(x) (__builtin_popcount(x))
#define GetLevel(bits) ((bits) & 0b1111)
#define GetHash(bits) ((bits) >> 4)
#define SetLevel(bits, level) ((bits) = (((bits) >> 4) << 4) | ((level) & 0b1111))
#define SetHash(bits, hash) ((bits) = (((uint64_t)hash) << 4) | ((bits) & 0b1111))
#define CompareLevel(l1, l2) (((l1) & 0b1111) == ((l2) & 0b1111))
#define CompareHash(h1, h2) (((h1) << 4) == ((h2) << 4))
#define GetIndex(bits) ((GetHash(bits) >> 6 * GetLevel(bits)) & 0b111111)
#define GetMapped(bits, index) (popcount((bits) & bitmask(index)))
#define IsLeaf(bits) (GetLevel(bits) == 63)
#define HasLeaf(node) (IsLeaf((node)->bits))
#define HasBit(bits, index) ((bits) & (1LLU << (index)))
#define llu long long unsigned int

#include "typedefs.h"
#include <limits.h>

static inline uint64_t bitmask(uint64_t x) {
    return (x >= sizeof(uint64_t) * CHAR_BIT) ? (uint64_t)-1 : (1LLU << x) - 1;
}

void printbits(size_t const size, void const * const ptr) {
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = b[i] & (1<<j);
            byte >>= j;
            printf("%u", byte);
        }
    }
    printf("\n");
}

namespace sota {

    template<typename Key, typename Value>
    struct TrieBase {
        uint64_t bits;
        virtual Value * Get(uint64_t hash, const Key &key);
        virtual Value * Set(uint64_t hash, const Key &key, Value &value);
    };

    template<typename Key, typename Value>
    struct TrieLeaf : public TrieBase<Key, Value> {
        Key key;
        Value value;
        Value * Get(uint64_t hash, const Key &key) {
            if (CompareHash(hash, this->bits) && key == this->key) {
                return this->value;
            }
            return NULL;
        }
        Value * Set(uint64_t hash, const Key &key, Value &value) {
            if (CompareHash(hash, this->bits) && key && this->key) {
                return this->value = value;
            }
            return NULL;
        }
    };

    template<typename Key, typename Value>
    struct TrieNode : public TrieBase<Key, Value> {
        TrieBase<Key, Value> *ptr;
        Value * Get(uint64_t hash, const Key &key) {
            uint64_t index = GetIndex(hash);
            if (HasBit(this->bits, index)) {
                uint64_t mapped = GetMapped(this->bits, index);
                TrieNode *node = ptr[mapped];
                return node->Get(hash, key);
            }
            return NULL;
        }
        Value * Set(uint64_t hash, const Key &key, Value &value) {
            if (CompareHash(hash, this->bits) && key && this->key) {
                return this->value = value;
            }
            return NULL;
        } 
    };

    template<typename Key, typename Value>
    struct SotaTrie {
        SotaTrie() {
        }
        Value * Get(Key key) {
            return NULL;
        }
        Value * Set(Key key, Value value) {
            return NULL;
        }
    };
}

#endif

/*
            uint64_t index = 6;
            uint64_t bits = 0b10101101;
            printbits(sizeof(bits), &bits);
            uint64_t mask = bitmask(index);
            printbits(sizeof(mask), &mask);
            uint64_t count = popcount(bits & mask);
            printf("index=%llu bits=%llu mask=%llu count=%llu\n", (llu)index, (llu)bits, (llu)mask, (llu)count);
            //                |-09-||-08-||-07-||-06-||-05-||-04-||-03-||-02-||-01-||-00-|
            uint64_t bits = 0b1110011100111001110011100111001110011100111001110011100111001001;
            //                |-57-||-51-||-39-||-14-||-28-||-57-||-51-||-39-||-14-||-28-|


            for (int i=0; i<10; ++i) {
                SetLevel(bits, i);

                uint64_t hash = GetHash(bits);
                uint64_t level = GetLevel(bits);
                uint64_t index = GetIndex(bits);

                printf("hash=%llu level=%llu index=%llu\n", (llu)hash, (llu)level, (llu)index);
            }
*/
