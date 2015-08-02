#ifndef JSONCPP_SERVICE_H
#define JSONCPP_SERVICE_H

#include <map>
#include <string>
#include <memory>

#include "channel.h"
#include "handler.h"
#include "request.h"

namespace core {
namespace jsonrpc {


class Service {
	std::map<std::string, std::unique_ptr<AbstractHandler>> _methods;
	std::map<Request*,std::shared_ptr<Channel>> _channels;

	public:
	void add_handler(const std::string &name, std::unique_ptr<AbstractHandler>&&handler);
	void remove_handler(const std::string &name);

	void process(std::shared_ptr<Channel>c, const std::string &message);

	protected:
	void on_request_completed(Request *r);
	void process_single_entry(Request &request, const Json::Value &entry);
	bool validate_request(const Json::Value &request);
	void clean_up(Request *r);

};


}
}

#endif
