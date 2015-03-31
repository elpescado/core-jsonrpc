#include "request.h"

using namespace code::jsonrpc;

Request::Request()
{
}

Request::Request(bool return_as_array)
	: _return_as_array{return_as_array}
{
}

void Request::on_result_set(std::shared_ptr<Result>)
{
	++_completed;
	if(completed()) {
		signal_completed();
	}
}


void Request::add(Handler *h, const Json::Value &id, const Json::Value &args)
{
	auto result = std::make_shared<Result>(id);
	result->signal_state_changed.connect(
			boost::bind(&Request::on_result_set, this, result)
	);
	_results.push_back(result);
	h->invoke(result, args);
}


void Request::add(std::shared_ptr<Result> result)
{
	if (result->state() != ResultState::None)
		_completed++;
	result->signal_state_changed.connect(
			boost::bind(&Request::on_result_set, this, result)
	);
	_results.push_back(result);
}
