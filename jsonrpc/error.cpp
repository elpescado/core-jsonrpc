#include "error.h"

using namespace core::jsonrpc;

Error::Error(int code, const std::string &message)
	: _code{code}, _message{message}
{
}

