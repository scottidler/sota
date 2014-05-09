#ifndef __SOTA_UTILS__
#define __SOTA_UTILS__ = 1

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

namespace sota {
    namespace utils {

        typedef unsigned int uint;

        vector<string> get_lines(string filename);
        vector<string> split(const string &s, char delim, bool empties);
        vector<string> &split(const string &s, char delim, vector<string> &elems, bool empties);

        template<class Key, class Value>
        vector<Key> keys(map<Key, Value> m) {
            vector<Key> keys = {};
            map<Key, Value>::iterator iter;
            for (iter = m.begin(); iter != m.end(); ++iter)
                keys.push_back(iter->first);
            return keys;
        }

        template<class Key, class Value>
        vector<Value> values(map<Key, Value> m) {
            vector<Value> values = {};
            map<Key, Value>::iterator iter;
            for (iter = m.begin(); iter != m.end(); ++iter)
                keys.push_back(iter->second);
            return keys;
        }

        bool startof(const string &prefix, const string &str);
        bool startofany(const string &prefix, const vector<string> &strs);

    }
}

#endif /*__SOTA_UTILS__*/