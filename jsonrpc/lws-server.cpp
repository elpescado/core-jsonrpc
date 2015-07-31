#include "lws-server.h"
#include <iostream>

#include <libwebsockets.h>

using namespace core::jsonrpc;

const char *reasons[] = {
	"LWS_CALLBACK_ESTABLISHED",
	"LWS_CALLBACK_CLIENT_CONNECTION_ERROR",
	"LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH",
	"LWS_CALLBACK_CLIENT_ESTABLISHED",
	"LWS_CALLBACK_CLOSED",
	"LWS_CALLBACK_CLOSED_HTTP",
	"LWS_CALLBACK_RECEIVE",
	"LWS_CALLBACK_CLIENT_RECEIVE",
	"LWS_CALLBACK_CLIENT_RECEIVE_PONG",
	"LWS_CALLBACK_CLIENT_WRITEABLE",
	"LWS_CALLBACK_SERVER_WRITEABLE",
	"LWS_CALLBACK_HTTP",
	"LWS_CALLBACK_HTTP_BODY",
	"LWS_CALLBACK_HTTP_BODY_COMPLETION",
	"LWS_CALLBACK_HTTP_FILE_COMPLETION",
	"LWS_CALLBACK_HTTP_WRITEABLE",
	"LWS_CALLBACK_FILTER_NETWORK_CONNECTION",
	"LWS_CALLBACK_FILTER_HTTP_CONNECTION",
	"LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED",
	"LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION",
	"LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS",
	"LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS",
	"LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION",
	"LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER",
	"LWS_CALLBACK_CONFIRM_EXTENSION_OKAY",
	"LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED",
	"LWS_CALLBACK_PROTOCOL_INIT",
	"LWS_CALLBACK_PROTOCOL_DESTROY",
	"LWS_CALLBACK_WSI_CREATE",
	"LWS_CALLBACK_WSI_DESTROY",
	"LWS_CALLBACK_GET_THREAD_ID",
	"LWS_CALLBACK_ADD_POLL_FD",
	"LWS_CALLBACK_DEL_POLL_FD",
	"LWS_CALLBACK_CHANGE_MODE_POLL_FD",
	"LWS_CALLBACK_LOCK_POLL",
	"LWS_CALLBACK_UNLOCK_POLL",

	"LWS_CALLBACK_OPENSSL_CONTEXT_REQUIRES_PRIVATE_KEY"
};
static int
callback_ws  (struct libwebsocket_context *context,
              struct libwebsocket *wsi,
			  enum libwebsocket_callback_reasons reason,
			  void *user,
			  void *in,
			  size_t len)
{
	LwsServer *server = static_cast<LwsServer*>(libwebsocket_context_user(context));
	if (reason != 30) {
		std::cerr << "Activity on lws " << server << " wsi " << wsi << " reason = " << reasons[reason] << " (" << reason << ")"
			<< std::endl;
	}
	return server->callback(wsi, reason, in, len);
}


static struct libwebsocket_protocols protocols[] = {
	{ "ws",   callback_ws,   sizeof(int), 16*1024 },
	{ NULL,   NULL, 0, 0 },
};


LwsServer::LwsServer(int port)
	: _port(port)
{
	struct lws_context_creation_info info = {0};
	info.uid = -1;
	info.gid = -1;
	info.iface = "lo0";
	info.port = _port;
	info.protocols = protocols;
	info.user = this;

	_context = libwebsocket_create_context(&info);
}

int LwsServer::callback(struct libwebsocket *wsi, int reason, void *in, size_t len)
{
	switch (reason) {
		case LWS_CALLBACK_HTTP:
			libwebsockets_serve_http_file(_context, wsi,
					"index.html", "text/html", NULL, 0);
			break;
		case LWS_CALLBACK_ESTABLISHED:
			on_callback_estabilished(wsi);
			break;
		case LWS_CALLBACK_RECEIVE:
			return on_readable(wsi,(unsigned char*)in,len);
//		case LWS_CALLBACK_SERVER_WRITEABLE:
//			return on_writable(wsi);
	}
	return 0;
}


void LwsServer::on_callback_estabilished(struct libwebsocket *wsi)
{
	std::cerr << "Callback estabilished" << std::endl;
// 	_client_sockets.emplace(wsi, );
	_client_sockets[wsi] = std::unique_ptr<LwsSocket>(new LwsSocket(wsi));
 }

int LwsServer::on_readable(struct libwebsocket *wsi, unsigned char*data, int len)
{
	if (_client_sockets.find(wsi) != _client_sockets.end()) {
		auto &socket = _client_sockets[wsi];
		socket->on_message((char*)data, (int)len);
	}
	return 0;
}


std::unique_ptr<Socket> LwsServer::accept()
{
	return NULL;
}

void LwsServer::run()
{
	for(;;) {
		libwebsocket_service(_context, 50);
	}
}


// SOCKET

LwsSocket::LwsSocket(struct libwebsocket *wsi)
	: _wsi(wsi)
{
	write_data("ELO", 3);
}

int64_t LwsSocket::write_data(const char* buffer, int64_t l)
{
	char *buf = new char[LWS_SEND_BUFFER_PRE_PADDING
					+ LWS_SEND_BUFFER_POST_PADDING
					+ l];

	std::memcpy(buf+LWS_SEND_BUFFER_PRE_PADDING, buffer, l);
	libwebsocket_write(_wsi, (unsigned char*)(LWS_SEND_BUFFER_PRE_PADDING+buf), l, LWS_WRITE_TEXT); 
	delete[] buf;

	return 0;
}


int64_t LwsSocket::read_data(char *buffer, int64_t l)
{
	return 0;
}
