#include "result.h"

using namespace code::jsonrpc;


Result::Result(const Json::Value &id)
	: _id{id}
{
}


void Result::set_result(const Json::Value &result)
{
	if(_state != ResultState::None)
		throw std::invalid_argument("");
	_result = result;
	_state = ResultState::Result;

	signal_state_changed();
}


void Result::set_error(const Error &error)
{
	if(_state != ResultState::None)
		throw std::invalid_argument("");
	_error = error;
	_state = ResultState::Error;

	signal_state_changed();
}

