#ifndef __SOTA_EXCEPTIONS__
#define __SOTA_EXCEPTIONS__ = 1

#include <cstring>
#include <iostream>
#include <exception>

namespace sota {
	class SotaException: public std::exception {
		std::string _message;
	public:
		SotaException(std::string message) {
			_message = message;
		}
		virtual const char * what() const throw() {
			return _message.c_str();
		}

	};
}

#endif /*__SOTA_EXCEPTIONS__*/
