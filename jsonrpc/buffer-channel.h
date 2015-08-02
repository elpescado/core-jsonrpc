#ifndef JSONRPC_BUFFER_CHANNEL_H
#define JSONRPC_BUFFER_CHANNEL_H

#include <string>

#include "channel.h"

class BufferChannel : public virtual core::jsonrpc::Channel
{
	std::string _content;

	public:
	BufferChannel();

	void send(const std::string &msg) override;
	std::string read();
	void reset();
};

#endif
