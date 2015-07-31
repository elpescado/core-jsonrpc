#ifndef JSONRPC_CLIENT_H
#define JSONRPC_CLIENT_H

#include <string>
#include <json/value.h>

#include "channel.h"

namespace core {
namespace jsonrpc {

class Client;


inline void argument_packer(Json::Value &array)
{
	// noop
}
/*
template<class T>
void argument_packer(Json::Value &array, T arg)
{
	array.append(arg);
}
*/
template<class T, typename... Args>
void argument_packer(Json::Value &array, T arg, Args... args)
{
	array.append(arg);
	argument_packer(array, args...);
}


class ProcedureBase {
	Client *_client;
	std::string _name;

	protected:
	ProcedureBase(Client &c, const std::string &name);
	void invoke(const Json::Value &args);
};

template<class X>
class RemoteProcedure;

template<typename R, typename... T>
class RemoteProcedure<R(T...)> : public ProcedureBase
{
	public:
	RemoteProcedure<R(T...)>(Client &c, const std::string &name)
		: ProcedureBase(c,name)
	{
	}

	R operator() (T... params) {
		Json::Value arguments;
		argument_packer(arguments, params...);
		invoke(arguments);
	}
};


class Client {

	int _next_id = 1;
	Channel *_channel;

	public:
	Client();
	Client(Channel &channel);
	
	/*
	template<typename X>
	RemoteProcedure<X> get_procedure(const std::string &name);
	*/
	template<typename F>
	RemoteProcedure<F> get_procedure(const std::string &name)
	{
		RemoteProcedure<F> proc(*this, name);
		return proc;
	}


	Json::Value call_method(const std::string &name, const Json::Value &arguments);

	void call_notification(const std::string &name, const Json::Value &arguments); 

	private:
	int next_id() { return _next_id++; }
	Json::Value create_request(const std::string &name, const Json::Value &arguments);

	void send_request(const Json::Value &request);
};


}
}

#endif
