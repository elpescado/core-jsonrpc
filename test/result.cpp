#include "gtest/gtest.h"

#include "result.h"
#include <json/value.h>

using namespace code::jsonrpc;

TEST(ResultTest, TestConstructor)
{
	Json::Value id(5);
	Result res(id);

	ASSERT_EQ(id, res.id());
	ASSERT_EQ(ResultState::None, res.state());
	ASSERT_THROW(res.result(), std::invalid_argument);
}


TEST(ResultTest, TestSetResult)
{
	Json::Value id(6);
	Json::Value result("A Test Result");
	Result res(id);

	res.set_result(result);
	ASSERT_EQ(ResultState::Result, res.state());
	ASSERT_EQ(result, res.result());
}


TEST(ResultTest, TestSetResultTwice)
{
	Json::Value id(6);
	Json::Value result("A Test Result");
	Result res(id);

	res.set_result(result);
	ASSERT_THROW(res.set_result(result), std::invalid_argument);
}


TEST(ResultTest, TestSignalStateChanged)
{
	bool fired = false;
	Json::Value id(7);
	Json::Value result("A Test Result");
	Result res(id);
	res.signal_state_changed.connect([&fired](){
			fired = true;
	});

	res.set_result(result);
	ASSERT_EQ(true, fired);
}


TEST(ResultTest, TestSetError)
{
	bool fired = false;
	Json::Value id("8");

	Result res(id);
	res.signal_state_changed.connect([&fired](){
			fired = true;
	});
	res.set_error(Error(42, "Malfunction"));
	ASSERT_EQ(true, fired);

	Error e = res.error();
	ASSERT_EQ(42, e.code());
	ASSERT_EQ(std::string("Malfunction"), e.message());
}
