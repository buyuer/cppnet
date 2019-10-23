#pragma once

#include "Http.hpp"
#include "Socket.hpp"

namespace test
{
	class Response : public HTTP
	{
	private:
		Socket* _socket;
	protected:

	public:
		string test;

		Response(Socket* s) :_socket(s) {

		}

		bool setBody(string body) {
			_socket->send("\r\n", 2);
			_socket->send(body.data(), body.length());
			return true;
		}

		bool setField(string name, string value) {
			_socket->send(name.data(), name.length());
			_socket->send(": ", 2);
			_socket->send(value.data(), value.length());
			_socket->send("\r\n", 2);
			return true;
		}

	};
}
