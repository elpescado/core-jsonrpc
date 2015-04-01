#include "gtest/gtest.h"

#include "error.h"

using namespace core::jsonrpc;

TEST(ErrorTest, TestConstructor)
{
	Error e(100, "Malfunction");

	ASSERT_EQ(100, e.code());
	ASSERT_EQ(std::string("Malfunction"), e.message());
}


TEST(ErrorTest, TestCopy)
{
	Error e(200, "Failure");
	Error copy(e);

	ASSERT_EQ(200, copy.code());
	ASSERT_EQ(std::string("Failure"), copy.message());
}
