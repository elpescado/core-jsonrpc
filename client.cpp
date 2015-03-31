#include <iostream>

#include <json/value.h>
#include <json/reader.h>


template<typename T>
void serialize_params(Json::Value &value, T param)
{
	value.append(Json::Value(param));	
}
template<typename T0, typename... T>
void serialize_params(Json::Value &value, T0 param0, T... params)
{
	value.append(Json::Value(param0));
	serialize_params<T...>(value, params...);
}


template<class X>
class RemoteProcedure;

template<typename R, typename... T>
class RemoteProcedure<R(T...)>
{
	public:
		R operator() (T... params) {
			Json::Value json_params;
			serialize_params(json_params, params...);
			std::cout << "f() params = " << json_params.toStyledString() << std::endl;
			return static_cast<R>(0);
		}
};


template<typename... T>
void f(T... t, int u)
{
}

int main(int argc, char **argv)
{
	RemoteProcedure<int(int)> f1;
	f1(4);
	f1(42);

	RemoteProcedure<int(double, double)> f2;
	f2(0.2, 0.3);
	f2(0.5, 0.7);

	f<int,int>(1,2,4);
}
