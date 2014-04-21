#ifndef __SOTA_UTILS__
#define __SOTA_UTILS__ = 1


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
    }
}

#endif /*__SOTA_UTILS__*/