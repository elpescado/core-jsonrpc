#include "handler.h"

using namespace core::jsonrpc;

Handler::Handler(std::function<void(std::shared_ptr<Result>, const Json::Value &)> function)
	: _func{function}
{
}

void Handler::invoke(std::shared_ptr<Result> r, const Json::Value &params)
{
	_func(r, params);
}
