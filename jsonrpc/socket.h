#ifndef SOCKET_H
#define SOCKET_H

namespace core {
namespace jsonrpc {

class Socket {
	public:
	virtual int64_t write_data(const char* buffer, int64_t l) = 0;
	virtual int64_t read_data(char *buffer, int64_t l) = 0;
};

}
}

#endif
