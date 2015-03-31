#include <string>

#include <json/value.h>
#include <json/reader.h>
#include "gtest/gtest.h"

#include "service.h"

using namespace code::jsonrpc;

class ServiceTestStub : public Service
{
	public:
	bool validate_request_public(const Json::Value &request)
	{
		return validate_request(request);
	}
};


TEST(ServiceTest, TestValidateCorrectRequest)
{
	ServiceTestStub service;
	Json::Value request;
	Json::Reader reader;
	std::string test_data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"params\": [42, 23], \"id\": 1}";

	ASSERT_TRUE(reader.parse(test_data, request));
	ASSERT_TRUE(service.validate_request_public(request));
}


TEST(ServiceTest, TestValidateCorrectNotification)
{
	ServiceTestStub service;
	Json::Value request;
	Json::Reader reader;
	std::string test_data = "{\"jsonrpc\": \"2.0\", \"method\": \"ping\"}";

	ASSERT_TRUE(reader.parse(test_data, request));
	ASSERT_TRUE(service.validate_request_public(request));
}


TEST(ServiceTest, TestValidateRequestNotObject)
{
	ServiceTestStub service;
	Json::Value request;
	Json::Reader reader;
	std::string test_data = "\"foobar(42)\"";

	ASSERT_TRUE(reader.parse(test_data, request));
	ASSERT_FALSE(service.validate_request_public(request));
}


TEST(ServiceTest, TestValidateMissingMethodName)
{
	ServiceTestStub service;
	Json::Value request;
	Json::Reader reader;
	std::string test_data = "{\"jsonrpc\": \"2.0\", \"params\": [42, 23], \"id\": 1}";

	ASSERT_TRUE(reader.parse(test_data, request));
	ASSERT_FALSE(service.validate_request_public(request));
}


TEST(ServiceTest, TestValidateIncorrectMethodName)
{
	ServiceTestStub service;
	Json::Value request;
	Json::Reader reader;
	std::string test_data = "{\"jsonrpc\": \"2.0\", \"method\": 42, \"params\": [42, 23], \"id\": 1}";

	ASSERT_TRUE(reader.parse(test_data, request));
	ASSERT_FALSE(service.validate_request_public(request));
}


TEST(ServiceTest, TestValidateIncorrectParams)
{
	ServiceTestStub service;
	Json::Value request;
	Json::Reader reader;
	std::string test_data = "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\", \"params\": 11, \"id\": 1}";

	ASSERT_TRUE(reader.parse(test_data, request));
	ASSERT_FALSE(service.validate_request_public(request));
}


