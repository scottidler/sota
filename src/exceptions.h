#ifndef __SOTA_EXCEPTIONS__
#define __SOTA_EXCEPTIONS__ = 1

#include <cstring>
#include <sstream>
#include <iostream>
#include <exception>

namespace sota {
    class SotaException: public std::exception {
        const char *_filename;
        size_t _line;
        const std::string _message;
        std::string _what;
    public:
        SotaException(const char *filename, size_t line, const std::string message = "")
            : _filename(filename)
            , _line(line)
            , _message(message) {
            std::ostringstream out;
            out << "filename=" << _filename
                << " line=" << _line
                << " msg=" << _message
                << std::endl;
            _what = out.str();
        }
        virtual const char * what() const throw() {
            return _what.c_str();
        }
    };

    class SotaNotImplemented: SotaException {
    public:
        SotaNotImplemented(const char *filename, size_t line, const std::string message = "")
            : SotaException(filename, line, message) {
        }
    };
}

#endif /*__SOTA_EXCEPTIONS__*/
