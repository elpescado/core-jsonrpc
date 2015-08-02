#include "gtest/gtest.h"

#include "buffer-channel.h"

using namespace core::jsonrpc;

TEST(BufferChannelTest, TestReadSentData) {
	BufferChannel c;
	c.send("Test Message");
	
	ASSERT_EQ("Test Message", c.read());
}


TEST(BufferChannelTest, TestReadAfterTwoWrites) {
	BufferChannel c;
	c.send("MSG1");
	c.send("MSG2");

	ASSERT_EQ("MSG1MSG2", c.read());
}


TEST(BufferChannelTest, TestReadResetsBuffer) {
	BufferChannel c;

	c.send("foo");
	c.read();

	ASSERT_EQ("", c.read());
}


TEST(BufferChannelTest, TestReset) {
	BufferChannel c;

	c.send("foo");
	c.reset();

	ASSERT_EQ("", c.read());
}
