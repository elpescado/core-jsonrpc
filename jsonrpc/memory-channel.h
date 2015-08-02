#ifndef JSONRPC_MEMORY_CHANNEL_H
#define JSONRPC_MEMORY_CHANNEL_H

#include "channel.h"

class MemoryChannel : public virtual core::jsonrpc::Channel
{
	public:
	MemoryChannel(Channel * end = nullptr);

	void send(const std::string &msg) override;

	private:
	Channel *_end;
};


#endif

