#include <gtest/gtest.h>

#include <string>

#include <iostream>
#include <fstream>

#include <json/value.h>
#include <json/reader.h>

#include "channel.h"
#include "handler.h"
#include "service.h"

using namespace core::jsonrpc;

namespace json {
	class string {
		private:
			std::string str;

		public:
			string(const char *s) : str{s} {};
			string(std::string s) : str{s} {};

			bool operator== (const std::string &other) const
			{
				Json::Reader r;
				Json::Value r1, r2;
				r.parse(str, r1);
				r.parse(other, r2);
				return r1 == r2;
			}

			bool operator== (const json::string &other) const
			{
				return *this==other.str;
			}

			bool operator!= (const json::string &other) const
			{
				return !(*this==other.str);
			}

			friend std::ostream& operator<<(std::ostream &o, const json::string &s)
			{
				o << s.str;
				return o;
			}
	};

};


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
		x = params["subtrahend"].asInt();
		y = params["minuend"].asInt();
	}

	std::cout << "subtract("<<x<<","<<y<<")"<<std::endl;
	result->set_result(x-y);
}

static void update (std::shared_ptr<Result> result, const Json::Value &params)
{
	std::cout << "update()" << std::endl;
}
static void foobar (std::shared_ptr<Result> result, const Json::Value &params)
{
	std::cout << "fobar()" << std::endl;
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
		add_handler("update", make_unique<Handler>(update));
		add_handler("foobar", make_unique<Handler>(foobar));
	}
	std::string invoke (const std::string &s)
	{
		process(_channel, s);
		return _channel->response();
	}
};


TEST(IntegrationTest, TestSanity)
{
	ASSERT_EQ(
		json::string("{\"a\":\"b\", \"b\":\"a\"}"),
		json::string("{\"a\":\"b\", \"b\":\"a\"}")
	);
	ASSERT_EQ(
		json::string("{\"a\":\"b\", \"b\":\"a\"}"),
		json::string("{\"b\":\"a\", \"a\":\"b\"}")
	);
	ASSERT_NE(
		json::string("{\"a\":\"b\", \"b\":\"a\"}"),
		json::string("{\"b\":\"a\", \"a\":\"c\"}")
	);
}


TEST(IntegrationTest, TestParseError)
{
	TestService t;
	std::string rv = t.invoke("BAD JSON");


	ASSERT_EQ(
		json::string("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}"),
		json::string(rv)
	);
}


TEST(IntegrationTest, TestData)
{
	return;
	TestService t;

	Json::Value root;
	Json::Reader reader;

	std::ifstream f("test/integration.json");
	ASSERT_TRUE(reader.parse(f, root)) << reader.getFormattedErrorMessages();

	ASSERT_TRUE(root.isArray());

	for (auto test : root) {
		std::string name = test["name"].asString();
		Json::Value request = test["request"];
		std::string request_string = request.toStyledString();

		std::string rv = t.invoke(request_string);

		if(test.isMember("response")) {
			Json::Value expected = test["response"];
			Json::Value got;
			reader.parse(rv, got);
			EXPECT_TRUE(expected == got) << "Test " << name << " failed" <<std::endl<<"  Actual: " << got.toStyledString() << std::endl << "Expected: " << expected.toStyledString();
		}
		else {
			EXPECT_EQ(std::string(""), rv);
		}

	}
}

/*
void ServiceTest::test_invalid_request()
{
	Service srv;
	std::string rv = srv.process("{\"method\":\"f\", \"params\":\"fff\"}");

	CPPUNIT_ASSERT_EQUAL(
		json::string("{\"error\": {\"code\": -32600, \"message\": \"Invalid Request\"}, \"id\": null}"),
		json::string(rv)
	);
}


void ServiceTest::test_method_not_found()
{
	Service srv;
	std::string rv = srv.process("{\"method\":\"foobar\", \"params\":[2,3],\"id\":2}");

	CPPUNIT_ASSERT_EQUAL(
		json::string("{\"error\": {\"code\": -32601, \"message\": \"Method not found\"}, \"id\": null}"),
		json::string(rv)
	);
}


void ServiceTest::test_invalid_params()
{
	Service srv;
	srv.add("add", func_add);
	std::string rv = srv.process("{\"method\":\"add\", \"params\":[2],\"id\":2}");

	CPPUNIT_ASSERT_EQUAL(
		json::string("{\"error\": {\"code\": -32602, \"message\": \"Invalid params\"}, \"id\": 2}"),
		json::string(rv)
	);
}

*/
