#include "gtest/gtest.h"

#include "request.h"

using namespace core::jsonrpc;


TEST(RequestTest, TestCompleted)
{
	Request r;
	bool completed = false;
	r.signal_completed.connect([&](){
			completed = true;
	});

	Handler h([](std::shared_ptr<Result> , const Json::Value &){});
	r.add(std::make_shared<Result>(1));
	r.add(std::make_shared<Result>(2));

	r.results()[0]->set_result("ttt");
	ASSERT_EQ(false,completed);

	r.results()[1]->set_result("ttt");
	ASSERT_EQ(true,completed);
}


TEST(RequestTest, TestAddResult)
{
	Request req;
	bool completed = false;
	req.signal_completed.connect([&](){
			completed = true;
	});

	auto res = std::make_shared<Result>(5);
	res->set_result(7);
	req.add(res);
	ASSERT_TRUE(req.completed());

	auto res2 = std::make_shared<Result>("id42");
	req.add(res2);
	ASSERT_FALSE(req.completed());

	res2->set_result(4);
	ASSERT_TRUE(req.completed());
	ASSERT_TRUE(completed);
}
