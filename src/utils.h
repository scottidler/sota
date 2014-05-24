#ifndef __SOTA_UTILS__
#define __SOTA_UTILS__ = 1

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace sota {

    typedef unsigned int uint;

    std::vector<std::string> get_lines(std::string filename);
    std::vector<std::string> split(const std::string &s, char delim, bool empties);
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems, bool empties);

    template<class Key, class Value>
    std::vector<Key> keys(std::map<Key, Value> m) {
        std::vector<Key> keys = {};
        typename std::map<Key, Value>::iterator iter;
        for (iter = m.begin(); iter != m.end(); ++iter)
            keys.push_back(iter->first);
        return keys;
    }

    template<class Key, class Value>
    std::vector<Value> values(std::map<Key, Value> m) {
        std::vector<Value> values = {};
        typename std::map<Key, Value>::iterator iter;
        for (iter = m.begin(); iter != m.end(); ++iter)
            values.push_back(iter->second);
        return values;
    }

    bool startof(const std::string &prefix, const std::string &str);
    bool startofany(const std::string &prefix, const std::vector<std::string> &strs);

}

#endif /*__SOTA_UTILS__*/
