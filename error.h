#ifndef JSONRPC_ERROR
#define JSONRPC_ERROR

#include <string>
#include <json/value.h>

namespace code {
namespace jsonrpc {


class Error {
	int         _code;
	std::string _message;

	public:
	Error(int code, const std::string &message);

	int code() const noexcept { return _code; }
	std::string message() const noexcept { return _message; }
};


}
}

#endif
