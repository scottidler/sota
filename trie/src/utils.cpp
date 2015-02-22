#include "utils.h"
#include <algorithm>
#include <sys/stat.h>

namespace sota {

    bool exists(const char *filename) {
        struct stat buffer;
        return (stat (filename, &buffer) == 0);
    }

    bool exists(const std::string &filename) {
        struct stat buffer;
        return (stat (filename.c_str(), &buffer) == 0);
    }

    std::vector<std::string> get_lines(std::string filename) {
    	std::vector<std::string> lines;
    	std::ifstream file(filename);
    	std::string line;
        while (getline(file, line) ) {
            lines.push_back(line);
        }
        return lines;
    }

    std::vector<std::string> split(const std::string &s, char delim, bool empties) {
    	std::vector<std::string> elems;
        split(s, delim, elems, empties);
        return elems;
    }

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems, bool empties) {
    	std::stringstream ss(s);
    	std::string item;
        while (getline(ss, item, delim)) {
            if (!empties && item == "")
                continue;
            elems.push_back(item);
        }
        return elems;
    }

    bool startof(const std::string &prefix, const std::string &str) {
        return prefix.size() <= str.size() && str.compare((std::string::size_type)0, prefix.size(), prefix) == 0;
    }

    bool startofany(const std::string &prefix, const std::vector<std::string> &strs) {
        for (auto const &str : strs) {
            if (startof(prefix, (std::string)str))
                return true;
        }
        return false;
    }
}
