#include <fstream>
#include <iostream>

#include <json/value.h>
#include <json/reader.h>

#include "channel.h"
#include "handler.h"
#include "service.h"

using namespace code::jsonrpc;


template<class T, class...Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    std::unique_ptr<T> ret (new T(std::forward<Args>(args)...));
    return ret;
}

static void subtract (std::shared_ptr<Result> result, const Json::Value &params)
{
	int x=0;
	int y=0;

	if(params.isArray()) {
		x = params[0].asInt();
		y = params[1].asInt();
	}
	else if (params.isObject()) {
		y = params["subtrahend"].asInt();
		x = params["minuend"].asInt();
	}

	std::cout << "subtract("<<x<<","<<y<<")"<<std::endl;
	result->set_result(x-y);
}

static void sum (std::shared_ptr<Result> result, const Json::Value &params)
{
	int x = 0;
	for(auto y : params) {
		x += y.asInt();
	}
	result->set_result(x);
}
static void update (std::shared_ptr<Result> result, const Json::Value &params)
{
	std::cout << "update()" << std::endl;
}
static void foobar (std::shared_ptr<Result> result, const Json::Value &params)
{
	std::cout << "fobar()" << std::endl;
}
static void notify_hello (std::shared_ptr<Result> result, const Json::Value &params)
{
	std::cout << "notify_hello()" << std::endl;
}
static void get_data (std::shared_ptr<Result> result, const Json::Value &params)
{
	std::cout << "get_data()" << std::endl;
	Json::Value rv;
	rv.append("hello");
	rv.append(5);
	result->set_result(rv);
}

class TestChannel : public Channel {
	std::string _response;

	public:
	virtual void send (const std::string &message) override
	{
		_response = message;
	}

	std::string response() const noexcept { return _response; }
	void reset() { _response = ""; }
};

class TestService : public Service
{
	std::shared_ptr<TestChannel> _channel;
	public:
	TestService()
		: _channel(std::make_shared<TestChannel>())
	{
		add_handler("subtract", make_unique<Handler>(subtract));
		add_handler("sum", make_unique<Handler>(sum));
		add_handler("update", make_unique<Handler>(update));
		add_handler("foobar", make_unique<Handler>(foobar));
		add_handler("notify_hello", make_unique<Handler>(notify_hello));
		add_handler("notify_sum", make_unique<Handler>(notify_hello));
		add_handler("get_data", make_unique<Handler>(get_data));
	}
	std::string invoke (const std::string &s)
	{
		_channel->reset();
		process(_channel, s);
		return _channel->response();
	}
};


int main(int argc, char **argv)
{
	TestService t;

	Json::Value root;
	Json::Reader reader;

	std::ifstream f("test/integration.json");
	if(!reader.parse(f, root)) {
		std::cerr << reader.getFormattedErrorMessages() << std::endl;
		return 1;
	}

	if (!root.isArray()) {
		return 2;
	}
	int errors = 0;

	for (auto test : root) {
		std::string name = test["name"].asString();
		Json::Value request = test["request"];
		std::string request_string = request.toStyledString();

		std::cerr << "[*] " << name << "... ";

		std::string rv = t.invoke(request_string);

		if(test.isMember("response")) {
			Json::Value expected = test.isMember("response")? test["response"] : Json::Value();
			Json::Value got;
			reader.parse(rv, got);
			if (expected != got) {
				std::cerr << "fail" << std::endl;
				std::cerr <<std::endl<<"  Actual: " << got.toStyledString() << std::endl << "Expected: " << expected.toStyledString();
				errors++;
			}
			else {
				std::cerr << "ok" << std::endl;
			}
		}
		else {
			if(std::string("") != rv) {
				std::cerr << "fail" << std::endl;
				std::cerr <<std::endl<<"  Actual: " << rv << std::endl << "Expected: (null)";
				errors++;
			}
			else {
				std::cerr << "ok" << std::endl;
			}
		}
	}
	std::cerr << "Errors: " << errors << std::endl;

	return errors;
}
