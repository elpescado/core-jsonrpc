#ifndef LWS_SERVER_H
#define LWS_SERVER_H

#include <map>

#include "server.h"
#include "socket.h"

struct libwebsocket;
struct libwebsocket_context;

namespace core {
namespace jsonrpc {

class LwsSocket : virtual public Socket {
	struct libwebsocket *_wsi;
	
public:
	LwsSocket(struct libwebsocket *wsi);

	boost::signals2::signal<void(char *data, int length)> on_message;

	virtual int64_t write_data(const char* buffer, int64_t l) override;
	virtual int64_t read_data(char *buffer, int64_t l) override;

protected:
};

class LwsServer : virtual public Server {
	int _port;
	struct libwebsocket_context *_context;
	std::map<struct libwebsocket *, std::unique_ptr<LwsSocket>> _client_sockets;

	public:
	LwsServer(int port = 8080);
	int callback(struct libwebsocket *wsi, int reason, void *in, size_t len);

	void run();

	virtual std::unique_ptr<Socket> accept() override;

	protected:
	void on_callback_estabilished(struct libwebsocket *wsi);
	int on_readable(struct libwebsocket *wsi, unsigned char*data, int len);
};

}
}

#endif
