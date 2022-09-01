#include "pch.h"
#include "WSClient.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;

unsigned int currentMsg = 0;

client c;
bool isConnected;
websocketpp::connection_hdl CurrentConnection;
std::string currentUri;

void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
	std::cout << msg->get_payload() << std::endl;
}

void on_open(websocketpp::connection_hdl hdl) {
	CurrentConnection = hdl;
	isConnected = true;
	std::cout << "Connection opened" << std::endl;
	Identify();
}

void on_close(websocketpp::connection_hdl hdl) {
	isConnected = false;
	std::cout << "Connection closed" << std::endl;
	RetryConection();
}

void on_fail(websocketpp::connection_hdl hdl)
{
	isConnected = false;
	std::cout << "Connection failed" << std::endl;
	RetryConection();
}

void RetryConection()
{
	std::cout << "Retrying connection" << std::endl;
	c.reset();
	try {
		// Set logging to be pretty verbose (everything except message payloads)
		c.set_access_channels(websocketpp::log::alevel::all);
		c.clear_access_channels(websocketpp::log::alevel::frame_payload);
		c.set_error_channels(websocketpp::log::elevel::all);

		// Initialize ASIO
		//c.init_asio();		

		// Register our message handler
		c.set_message_handler(&on_message);
		c.set_open_handler(&on_open);
		c.set_close_handler(&on_close);
		c.set_fail_handler(&on_fail);

		websocketpp::lib::error_code ec;
		client::connection_ptr con = c.get_connection(currentUri, ec);
		if (ec) {
			std::cout << "could not create connection because: " << ec.message() << std::endl;
		}

		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		c.connect(con);


		// Start the ASIO io_service run loop
		// this will cause a single connection to be made to the server. c.run()
		// will exit when this connection is closed.
		c.run();
	}
	catch (websocketpp::exception const& e) {
		std::cout << e.what() << std::endl;
	}
}

void SendSwitchScene(std::string sceneName, std::string delayMs)
{
	PerformSendWithDelay(sceneName, delayMs);
}

void SendSwitchScene(std::string sceneName)
{
	PerformSend(sceneName);
}

void PerformSend(std::string sceneName)
{
	if (isConnected)
	{
		if (sceneName != "")
		{
			nlohmann::json js;
			js["op"] =  6;

			nlohmann::json jsreq;
			jsreq["requestType"] = "SetCurrentProgramScene";
			jsreq["requestId"] = std::to_string(++currentMsg);

			nlohmann::json jsreqdata;
			jsreqdata["sceneName"] = sceneName;

			jsreq["requestData"] = jsreqdata;
			js["d"] = jsreq;

			c.send(CurrentConnection, js.dump(), websocketpp::frame::opcode::text);
		}
	}
}

void Identify() {
	if (isConnected) {
		nlohmann::json jmain;
		jmain["op"] = 1;

		nlohmann::json j;
		j["rpcVersion"] = 1;
		// todo implement auth

		jmain["d"] = j;

		c.send(CurrentConnection, jmain.dump(), websocketpp::frame::opcode::text);
	}
}

void PerformSendWithDelay(std::string sceneName, std::string delayMs)
{
	if (isConnected)
	{
		if (sceneName != "")
		{
			nlohmann::json jmain;
			jmain["op"] = 8;

			nlohmann::json jd;
			jd["requestType"] = "RequestBatch";
			jd["requestId"] = std::to_string(++currentMsg);
			jd["executionType"] = 0;

			nlohmann::json jsleep;
			jsleep["requestType"] = "Sleep";
			jsleep["requestId"] = std::to_string(++currentMsg);

			nlohmann::json jsleepdata;
			jsleepdata["sleepMillis"] = std::stoi(delayMs);
			jsleep["requestData"] = jsleepdata;
			
			nlohmann::json jsscene;
			jsscene["requestType"] = "SetCurrentProgramScene";
			jsscene["requestId"] = std::to_string(++currentMsg);

			nlohmann::json jsscenedata;
			jsscenedata["sceneName"] = sceneName;
			jsscene["requestData"] = jsscenedata;

			jd["requests"] = { jsleep, jsscene };
			jmain["d"] = jd;

			/*
			nlohmann::json jss;
			jss["request-type"] = "Sleep";
			jss["message-id"] = std::to_string(++currentMsg);
			jss["sleepMillis"] = std::stoi(delayMs);

			nlohmann::json jsc;
			jsc["request-type"] = "SetCurrentScene";
			jsc["message-id"] = std::to_string(++currentMsg);
			jsc["scene-name"] = sceneName;

			nlohmann::json js;
			js["request-type"] = "ExecuteBatch";
			js["message-id"] = std::to_string(++currentMsg);
			js["requests"] = { jss, jsc };
			*/

			//std::cout << jmain.dump(4) << std::endl;
			c.send(CurrentConnection, jmain.dump(), websocketpp::frame::opcode::text);
		}
	}
}

void RunServer(std::string uri)
{
	currentUri = uri;
	try {
		// Set logging to be pretty verbose (everything except message payloads)
		c.set_access_channels(websocketpp::log::alevel::all);
		c.clear_access_channels(websocketpp::log::alevel::frame_payload);
		c.set_error_channels(websocketpp::log::elevel::all);

		// Initialize ASIO
		c.init_asio();

		// Register our message handler
		c.set_message_handler(&on_message);
		c.set_open_handler(&on_open);
		c.set_close_handler(&on_close);
		c.set_fail_handler(&on_fail);

		websocketpp::lib::error_code ec;
		client::connection_ptr con = c.get_connection(uri, ec);
		if (ec) {
			std::cout << "could not create connection because: " << ec.message() << std::endl;
		}

		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		c.connect(con);


		// Start the ASIO io_service run loop
		// this will cause a single connection to be made to the server. c.run()
		// will exit when this connection is closed.
		c.run();
	}
	catch (websocketpp::exception const& e) {
		std::cout << e.what() << std::endl;
	}
}