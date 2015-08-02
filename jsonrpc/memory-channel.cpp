#include "memory-channel.h"

using namespace core::jsonrpc;

MemoryChannel::MemoryChannel(Channel *end)
	: _end(end)
{
}


void MemoryChannel::send(const std::string &msg)
{
	if(_end)
		_end->signal_message(msg);
}
