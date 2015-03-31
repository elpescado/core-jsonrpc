#include "service.h"

#include <json/value.h>
#include <json/reader.h>

#include <iostream>

using namespace code::jsonrpc;

void Service::add_handler(const std::string &name, std::unique_ptr<AbstractHandler>&& handler)
{
	_methods.emplace(name,std::move(handler));
}

void Service::remove_handler(const std::string &name)
{
	_methods.erase(name);
}


void Service::process(std::shared_ptr<Channel>c, const std::string &message)
{
	Json::Value root;
	Json::Reader reader;
	
	if(! reader.parse(message, root)) {
		c->send("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}");
		return;
	}

	if(root.isArray() && root.size() == 0) {
		c->send("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32600, \"message\": \"Invalid Request\"}, \"id\": null}");
		return;
	}


	auto request = new Request(root.isArray());
	_channels[request] = c;

	if(root.isArray()) {
		for (auto entry: root) {
			process_single_entry(*request, entry);
		}
	}
	else if (root.isObject()) {
		process_single_entry(*request, root);
	}

	/* Do nothing if there is nothing to return */
	if (request->empty()) {
		/* TODO: somehow signal this condition
		   to HTTP-like transports */
		clean_up(request);
		return;
	}

	/* If all results have been set by now
	   call completion handler, otherwise
	   set up a event listener */
	if (request->completed()) {
		on_request_completed(request);
	} else {
		request->signal_completed.connect(
			boost::bind(&Service::on_request_completed, this, request)
		);
	}
}


void Service::process_single_entry(Request &request, const Json::Value &entry)
{	
	if(!validate_request(entry)) {
		auto result = std::make_shared<Result>(Json::Value(Json::nullValue));
		result->set_error(Error(-32600, "Invalid Request"));
		request.add(result);
		return;
	}
	bool notification = !entry.isMember("id");
	Json::Value id = entry["id"];
	std::string method_name = entry["method"].asString();
	Json::Value params = entry["params"];

	if (_methods.find(method_name) == _methods.end()) {
		auto result = std::make_shared<Result>(id);
		result->set_error(Error(-32601, "Method not found"));
		request.add(result);
		return;
	}
	auto &handler = _methods[method_name];
	auto result = std::make_shared<Result>(id);

	if (!notification) {
		request.add(result);
	}
	handler->invoke(result, params);
}

bool Service::validate_request(const Json::Value &request)
{
	if (!request.isObject())
		return false;

	if (!request.isMember("method"))
		return false;

	if (!request["method"].isString())
		return false;

	if (!request["params"].isObject() && !request["params"].isArray())
		return false;

	return true;
}


void Service::on_request_completed(Request *r)
{
	Json::Value response{Json::arrayValue};
	for (auto result : r->results()) {
		if (result->state() == ResultState::Result) {
			Json::Value rv{Json::objectValue};
			rv["result"] = result->result();
			rv["id"] = result->id();
			rv["jsonrpc"] = "2.0";
			response.append(rv);
		}
		else if (result->state() == ResultState::Error) {
			Json::Value error{Json::objectValue};
			error["code"] = result->error().code();
			error["message"] = result->error().message();
			Json::Value rv{Json::objectValue};
			rv["error"] = error;
			rv["id"] = result->id();
			rv["jsonrpc"] = "2.0";
			response.append(rv);
		}
	}

	/* Tell a single request from one-item batch */
	if(!r->return_as_array()) {
		response = response[0];
	}

	std::shared_ptr<Channel>c = _channels[r];
	c->send(response.toStyledString());

	clean_up(r);
}

void Service::clean_up(Request *r)
{
	_channels.erase(r);
	delete r;
}
