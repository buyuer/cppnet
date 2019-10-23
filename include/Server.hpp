#pragma once

#include "Request.hpp"
#include "Response.hpp"

namespace test
{
	using Handle = void (*)(Request&, Response&);

	class Server
	{
	private:
		map<string, Handle> routeTable;
		Socket _server;

		void _handler(Socket&& s) {
			Request request;
			Response response(&s);
			char buff[1024];
			s.recv(buff, sizeof(buff));
			char line[] = "HTTP/1.1 200 OK\r\n";
			s.send(line, sizeof(line) - 1);
			routeTable["/"](request, response);
		}
	protected:
	public:
		Server() {
			Socket::init();
		}

		bool listen(int port) {
			return _server.bind("127.0.0.1", port);
		}

		void start() {
			_server.listen();
			while (true) {
				Socket s = _server.accept();
				std::thread h(&Server::_handler, this, std::move(s));
				h.join();
			}
		}

		bool setRoute(string url, Handle h) {
			routeTable.insert(pair<string, Handle>(url, h));
			return true;
		}
	};
}
