#include "buffer-channel.h"

using namespace core::jsonrpc;

BufferChannel::BufferChannel()
	: _content("")
{
}


void BufferChannel::send(const std::string &msg)
{
	_content += msg;
}


std::string BufferChannel::read()
{
	auto tmp = _content;
	_content = "";
	return tmp;
}


void BufferChannel::reset()
{
	_content = "";
}
