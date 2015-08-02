#include "gtest/gtest.h"

#include "memory-channel.h"

using namespace core::jsonrpc;

TEST(MemoryChannelTest, TestWrite) {

	MemoryChannel dest;
	MemoryChannel src(&dest);

	std::string received;
	dest.signal_message.connect([&](std::string msg) {
			received = msg;
	});

	src.send("Test message");

	ASSERT_EQ("Test message", received);
}
