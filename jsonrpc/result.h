#ifndef JSONCPP_RESULT_H
#define JSONCPP_RESULT_H

#include <stdexcept>

#include <json/value.h>
#include <boost/signals2.hpp>
#include <boost/optional.hpp>

#include "error.h"

namespace code {
namespace jsonrpc {

enum class ResultState {
	None,
	Result,
	Error
};

class Result {
	Json::Value _id;
	ResultState _state = { ResultState::None };
	Json::Value _result;
	boost::optional<Error> _error;

	public:
	Result(const Json::Value &id);

	Json::Value id() const noexcept { return _id; }
	ResultState state() const noexcept { return _state; }
	Json::Value result() const {
		if(_state != ResultState::Result)
			throw std::invalid_argument("");
		return _result;
	}
	Error       error() const { return *_error; }

	void set_result(const Json::Value &result);
	void set_error(const Error &error);

	boost::signals2::signal<void()> signal_state_changed;
};
	
}
}

#endif
