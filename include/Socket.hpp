#pragma once

#include "Base.hpp"

#if (defined _WIN32) || (defined _WIN64)
//Windows
#define WIN32_LEAN_AND_MEAN
//#include "windows.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#pragma comment (lib, "ws2_32.lib")
//Windows

namespace test
{
	class WinSocket
	{
	private:
		int _family;
		int _socktype;
		int _protocol;
		int _addrlen;
		SOCKADDR* _addr;
		SOCKET _socket;
	protected:

	public:
		using TYPE = enum
		{
			INET4 = AF_INET,
			INET6 = AF_INET6,
			STREAM = SOCK_STREAM,
			DGRAM = SOCK_DGRAM,
		};

		static void init()
		{
			WSADATA wsaData;
			WORD sockVersion = MAKEWORD(2, 2);
			if (WSAStartup(sockVersion, &wsaData) != 0) {
				return;
			}
		}

		explicit WinSocket(int pf = STREAM) :
			_socket(INVALID_SOCKET),
			_family(NULL),
			_socktype(NULL),
			_protocol(NULL),
			_addrlen(NULL),
			_addr(NULL) {
			_socktype = pf;
		};

		WinSocket(const WinSocket& wsock) = delete;

		WinSocket(WinSocket&& wsock) noexcept :WinSocket() {
			memcpy(this, &wsock, sizeof(WinSocket));
			wsock._socket = INVALID_SOCKET;
			wsock._addr = NULL;
		}

		WinSocket& operator = (const WinSocket& wsock) = delete;

		WinSocket& operator = (WinSocket&& wsock)noexcept {
			memcpy(this, &wsock, sizeof(WinSocket));
			wsock._socket = INVALID_SOCKET;
			wsock._addr = NULL;
		}

		~WinSocket() {
			if (_socket != INVALID_SOCKET) {
				this->close();
			}
			if (_addr != NULL) {
				delete[]_addr;
			}
		};

		bool bind(string ip, int port) {
			ADDRINFO* result = NULL, hints;
			ZeroMemory(&hints, sizeof(hints));
			//hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = _socktype;
			hints.ai_protocol = _protocol;
			if (getaddrinfo(ip.data(), to_string(port).data(), &hints, &result) != 0) {
				cout << "ip or port is error" << endl;
				return false;
			}
			_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (_socket == INVALID_SOCKET) {
				cout << "socket failed" << endl;
				freeaddrinfo(result);
				return false;
			}
			if (_socktype == SOCK_STREAM) {
				if (::bind(_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
					cout << "bind error\n" << endl;
					return false;
				}
			}
			_family = result->ai_family;
			_addrlen = result->ai_addrlen;
			_addr = (SOCKADDR*)(new char[_addrlen]);
			_addr = (SOCKADDR*)std::memcpy(_addr, result->ai_addr, _addrlen);

			freeaddrinfo(result);
			return true;
		}

		bool listen(int backlog = SOMAXCONN) {
			if (::listen(_socket, backlog) == SOCKET_ERROR) {
				cout << "listen error" << endl;
				return false;
			}
			return true;
		}

		bool connect() {
			if (::connect(_socket, _addr, _addrlen) == SOCKET_ERROR) {
				cout << "connect error" << endl;
				return false;
			}
		}

		bool connect(string ip, int port) {
			ADDRINFO hints, * result = NULL;
			ZeroMemory(&hints, sizeof(hints));
			hints.ai_protocol = _protocol;
			hints.ai_socktype = _socktype;
			if (getaddrinfo(ip.data(), to_string(port).data(), &hints, &result) != 0) {
				cout << "ip or port is error" << endl;
				freeaddrinfo(result);
				return false;
			}
			_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (_socket == INVALID_SOCKET) {
				cout << "socket failed" << endl;
				freeaddrinfo(result);
				return false;
			}
			if (::connect(_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
				cout << "connect error" << endl;
				freeaddrinfo(result);
				return false;
			};
			freeaddrinfo(result);
			return true;
		}

		WinSocket accept() {
			WinSocket temp(_socktype);
			memcpy(&temp, this, sizeof(WinSocket));
			temp._addr = (SOCKADDR*)(new char[_addrlen]);
			temp._socket = ::accept(_socket, temp._addr, NULL);
			if (temp._socket == INVALID_SOCKET) {
				closesocket(_socket);
				cout << "accept error" << endl;
				temp._socket = INVALID_SOCKET;
				return temp;
			}
			return temp;
		}

		int send(const char* data, int n) {
			return ::send(_socket, data, n, NULL);
		}

		int recv(char* buf, int n) {
			return ::recv(_socket, buf, n, NULL);
		}

		int sendto(const char* buf, int n) {
			return ::sendto(_socket, buf, n, NULL, _addr, _addrlen);
		}

		int sendto(const char* buf, int n, string ip, int port) {

		}

		int recvfrom(char* buf, int n) {
			return ::recvfrom(_socket, buf, n, NULL, _addr, &_addrlen);
		}

		int recvfrom(char* buf, int n, string ip, int port) {

		}

		void close() {
			::closesocket(_socket);
			_socket = INVALID_SOCKET;
		}

		string getip() {

		}

		int getport() {

		}
	};

	using Socket = test::WinSocket;
}
#endif
