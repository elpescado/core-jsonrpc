#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <boost/signals2.hpp>

namespace core{
namespace jsonrpc {

class Socket;

class Server {

	public:
	
	boost::signals2::signal<void(void)> signal_connect();

	virtual std::unique_ptr<Socket> accept() = 0;
};

}
}

#endif
