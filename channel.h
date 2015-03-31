#ifndef JSONRPC_CHANNEL_H
#define JSONRPC_CHANNEL_H

#include <string>

#include <boost/signals2.hpp>

namespace code {
namespace jsonrpc {


class Channel {
	public:
	virtual void send(const std::string &) = 0;

	boost::signals2::signal<void(std::string)> signal_message;
};

	
}
}

#endif
