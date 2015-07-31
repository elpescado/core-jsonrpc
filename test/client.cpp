#include "gtest/gtest.h"

#include "client.h"

using namespace core::jsonrpc;

TEST(ClientTest, TestGetProcedure)
{
	Client c;
	auto proc = c.get_procedure<int(int,int)>("add");
	proc(2,3);
	
	Json::Value args;
	args.append(1);
	args.append(2);
	c.call_method("add", args);
//	c.get_procedure<int>("foo");
}


TEST(ClientTest, TestArgumentPackerOneElementArray)
{
	Json::Value array;

	argument_packer(array, 42);

	Json::Value expect;
	expect.append(42);

	ASSERT_EQ(expect, array);
}


TEST(ClientTest, TestArgumentPackerArray)
{
	Json::Value array;

	argument_packer(array, 1, 2, 3);

	Json::Value expect;
	expect.append(1);
	expect.append(2);
	expect.append(3);

	ASSERT_EQ(expect, array);
}


TEST(ClientTest, TestArgumentPackerEmpty)
{
	Json::Value array;

	argument_packer(array);

	Json::Value expect;

	ASSERT_EQ(expect, array);
}

TEST(ClientTest, TestArgumentPackerStdString)
{
	Json::Value array;

	argument_packer(array, "arg1", "arg2");

	Json::Value expect;
	expect.append("arg1");
	expect.append("arg2");

	ASSERT_EQ(expect, array);
}


