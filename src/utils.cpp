#include "utils.h"

namespace sota {
    namespace utils {
        vector<string> get_lines(string filename) {
            vector<string> lines;
            ifstream file(filename);
            string line;
            while (getline(file, line) ) {
                lines.push_back(line);
            }
            return lines;
        }
        vector<string> split(const string &s, char delim, bool empties) {
            vector<string> elems;
            split(s, delim, elems, empties);
            return elems;
        }
        vector<string> &split(const string &s, char delim, vector<string> &elems, bool empties) {
            stringstream ss(s);
            string item;
            while (getline(ss, item, delim)) {
                if (!empties && item == "")
                    continue;
                elems.push_back(item);
            }
            return elems;
        }        
    }
}