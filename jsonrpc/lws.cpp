#include "lws-server.h"

using namespace core::jsonrpc;

int main()
{
	LwsServer server;
	server.run();
}
