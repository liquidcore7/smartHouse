//
// Created by liquidcore7 on 3/31/17.
//

#include "server_http.hpp"
#include "../GPIO/PinCtrl.hpp"
#include "../log.hpp"

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
const char* pswd = "raspHouse";

class shServer
{
	HttpServer server;
	std::vector<std::string> loggedIPs;
	bool authenticated(const std::string&);
	log_service logger;
public:
	shServer(PinCtrl &, int = 8080);
	inline void start() {log_message("Server started", 'S', logger); server.start();}
	inline void stop() {log_message("Server stopped", 'S', logger);server.stop();}
};

bool shServer::authenticated(const std::string &ip) {
	return (std::find(loggedIPs.begin(), loggedIPs.end(), ip) != loggedIPs.end());
}

shServer::shServer(PinCtrl &pinboard, int port) : loggedIPs{} {
	server.config.port = port;
	server.resource["^/auth$"]["POST"] = [&pswd, this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		bool authSuccessfull = (request->content.string() == pswd);
		if (authSuccessfull)
		{
			if (authenticated(request->remote_endpoint_address))
			{
				log_message("Authenticated user tried to log in", 'E', logger);
				*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << 28 <<
			              "\r\n\r\n" << "You`re already logged in" << "\r\n\r\n";
			}
			else {
				log_message("User authenticated", 'S', logger);
				loggedIPs.push_back(request->remote_endpoint_address);
				*response << "HTTP/1.1 200 OK\r\nContent-Length: " << 5 << "\r\n\r\n" << 1
			              << "\r\n\r\n";
			} //if (authenticated)
		}
		else
		{
			log_message("User failed to authenticate", 'E', logger);
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << 29 <<
		              "\r\n\r\n" << "Wrong password or request" << "\r\n\r\n";
		}
	};
	server.resource["^/setlight1$"]["POST"] = [&pinboard, this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		if (authenticated(request->remote_endpoint_address)) {
			int val;
			if (request->content.string() == "ON")
				val = 1;
			else if (request->content.string() == "OFF")
				val = 0;
			else
				val = -1;
			if (val > -1) {
				pinboard.getPin(2)->set(val);
				log_message("User switched light1", 'S', logger);
				*response << "HTTP/1.1 200 OK\r\nContent-Length: " << 5 << "\r\n\r\n" << val
			              << "\r\n\r\n";
			} else
			{
				log_message("Wrong light state requested", 'E', logger);
				*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << 34 <<
				          "\r\n\r\n" << "Only ON and OFF states allowed" << "\r\n\r\n";
			}
		} else {
			log_message("Unauthorized user tried to toggle light", 'E', logger);
			*response << "HTTP/1.1 401 Unauthorized\r\nContent-Length: " <<
		              41 << "\r\n\r\nAuthentication needed to toggle light" << "\r\n\r\n";
		}
	};
	server.resource["^/setlight2$"]["POST"] = [&pinboard, this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		if (authenticated(request->remote_endpoint_address)) {
			int val;
			if (request->content.string() == "ON")
				val = 1;
			else if (request->content.string() == "OFF")
				val = 0;
			else
				val = -1;
			if (val > -1) {
				pinboard.getPin(3)->set(val);
				log_message("User switched light2", 'S', logger);
				*response << "HTTP/1.1 200 OK\r\nContent-Length: " << 5 << "\r\n\r\n" << val
			              << "\r\n\r\n";
			} else
			{
				log_message("Wrong light state requested", 'E', logger);
				*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << 34 <<
				          "\r\n\r\n" << "Only ON and OFF states allowed" << "\r\n\r\n";
			}
		} else {
			log_message("Unauthorized user tried to toggle light", 'E', logger);
			*response << "HTTP/1.1 401 Unauthorized\r\nContent-Length: " <<
			          41 << "\r\n\r\nAuthentication needed to toggle light" << "\r\n\r\n";
		}
	};
	server.resource["^/setdoor$"]["POST"] = [&pinboard, this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		if (authenticated(request->remote_endpoint_address)) {
			int val;
			if (request->content.string() == "OPEN")
				val = 0;
			else if (request->content.string() == "CLOSE")
				val = 1;
			else
				val = -1;
			if (val > -1) {
				pinboard.getPin(4)->set(val);
				log_message("User opened/closed the door", 'S', logger);
				*response << "HTTP/1.1 200 OK\r\nContent-Length: " << 5 << "\r\n\r\n" << val
			              << "\r\n\r\n";
			} else
			{
				log_message("Wrong door state requested", 'E', logger);
				*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << 38 <<
				          "\r\n\r\n" << "Only OPEN and CLOSE states allowed" << "\r\n\r\n";
			}
		} else {
			log_message("Unauthorized user tried to open/close the door", 'E', logger);
			*response << "HTTP/1.1 401 Unauthorized\r\nContent-Length: " <<
			          52 << "\r\n\r\nAuthentication needed to open and close the door"
		              << "\r\n\r\n";
		}
	};
	server.resource["^/try$"]["GET"] = [this] (std::shared_ptr<HttpServer::Response> response,
	                                       std::shared_ptr<HttpServer::Request> request)
	{
		std::string hello = "Connected";
		log_message("Connection tested", 'S', logger);
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << hello.length() + 4
	              << "\r\n\r\n" << hello << "\r\n\r\n";
	};
	server.resource["^/getstate$"]["GET"] = [&pinboard, this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		log_message("Pin states requested", 'S', logger);
		bool currState[3] = {pinboard.getPin(2)->getState(),
		pinboard.getPin(3)->getState(), pinboard.getPin(4)->getState()};
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << 7
	              << "\r\n\r\n" << currState[0] << currState[1] << currState[2] << "\r\n\r\n";
	};
	server.resource["^/unauth$"]["POST"] = [this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		auto requestedDel = std::find(loggedIPs.begin(), loggedIPs.end(), request->remote_endpoint_address);
		if (requestedDel != loggedIPs.end())
		{
			loggedIPs.erase(requestedDel);
			log_message("User logged out", 'S', logger);
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " << 5 <<
			          "\r\n\r\n" << 1 << "\r\n\r\n";
		} else {
			log_message("Unauthorized user tried to log out", 'E', logger);
			*response << "HTTP/1.1 401 Unauthorized\r\nContent-Length: "
		              << 37 << "\r\n\r\n" << "Only authorized users can log out" << "\r\n\r\n";
		}
	};
	server.resource["^/exit$"]["POST"] = [this]
			(std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		if (request->content.string() == "exit_token_dbg")
		{
			log_message("Shutdown signal received", 'S', logger);
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " <<
			          5 << "\r\n\r\n" << 1 << "\r\n\r\n";
			stop();
		} else
		{
			log_message("Wrong shutdown token entered", 'E', logger);
			*response << "HTTP/1.1 403 Forbidden\r\nContent-Length: " <<
		            24  << "\r\n\r\n" << "Wrong shutdown token" << "\r\n\r\n";
		}
	};
};
