#ifndef JSONRPC_HANDLER_H
#define JSONRPC_HANDLER_H

#include <functional>
#include <memory>

#include <json/value.h>

#include "result.h"

namespace core {
namespace jsonrpc {

class AbstractHandler
{
	public:
	virtual void invoke(std::shared_ptr<Result> r, const Json::Value &params) = 0;
};


class Handler : public virtual AbstractHandler
{
	std::function<void(std::shared_ptr<Result>, const Json::Value &)> _func;
	public:
	Handler(std::function<void(std::shared_ptr<Result>, const Json::Value &)> function);
	virtual void invoke(std::shared_ptr<Result> r, const Json::Value &params) override;
};

}
}

#endif
