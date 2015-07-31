#include "client.h"

#include <iostream>

using namespace core::jsonrpc;

Client::Client()
	: _channel(nullptr)
{
}


Client::Client(Channel &channel)
	: _channel(&channel)
{
}


Json::Value Client::call_method(const std::string &name,
		                        const Json::Value &arguments)
{
	std::cerr << "Calling method " << name << 	" args = " << arguments.toStyledString() << std::endl;
	Json::Value request = create_request(name, arguments);
	request["id"] = next_id();
	send_request(request);
	return Json::Value();
}

void Client::call_notification(const std::string &name, const Json::Value &arguments)
{
	std::cerr << "Calling notification " << name << 	" args = " << arguments.toStyledString() << std::endl;
	Json::Value request = create_request(name, arguments);
	send_request(request);
}


Json::Value Client::create_request(const std::string &name, const Json::Value &arguments)
{
	Json::Value root(Json::objectValue);
	root["jsonrpc"] = "2.0";
	root["method"] = name;
	root["params"] = arguments;
	return root;
}

void Client::send_request(const Json::Value &request)
{
	std::cerr << "--> " << request.toStyledString() << std::endl;

	if (_channel) {
		_channel->send(request.toStyledString());
	}
}


///

ProcedureBase::ProcedureBase(Client &c, const std::string &name)
	: _client(&c), _name(name)
{
}

void ProcedureBase::invoke(const Json::Value &args)
{
	_client->call_method(_name, args);
}


