#ifndef JSONRPC_REQUEST_H
#define JSONRPC_REQUEST_H

#include <memory>
#include <vector>

#include <boost/signals2.hpp>

#include "handler.h"
#include "result.h"

namespace code {
namespace jsonrpc {


class Request {
	std::vector<std::shared_ptr<Result>> _results;
	int _completed { 0 };
	bool _return_as_array { false };


	public:
	Request();
	Request(bool);
	const std::vector<std::shared_ptr<Result>> &results() const { return _results; }

	void add(Handler *h, const Json::Value &id, const Json::Value &args);
	void add(std::shared_ptr<Result> result);

	bool empty() const noexcept { return _results.empty(); }
	bool completed() const noexcept
	{ return _completed == _results.size(); }
	bool return_as_array() const noexcept { return _return_as_array; }

	boost::signals2::signal<void()> signal_completed;

	private:
	void on_result_set(std::shared_ptr<Result>);
};


}
}

#endif
