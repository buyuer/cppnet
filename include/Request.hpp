#pragma once

#include "Http.hpp"

namespace test {

	class Request : public HTTP
	{
	private:

	public:
		explicit Request() {

		}

		explicit Request(s8* data) {
			_parse(data);
		}

		string getMethod() {
			return getLine()[0];
		}

		string getURL() {
			return getLine()[1];
		}

		string getVersion() {
			return getLine()[2];
		}

		void test() {
			const s8* str = "GET / HTTP/1.1\r\nHost: 127.0.0.1:9999\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:69.0) Gecko/20100101 Firefox/69.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nCache-Control: max-age=0\r\n\r\n{\"ding\":123}";
			vector<basic_string<u8>> strs;
			//_splitStr<u8>(str, (u8*)"\r\n", strs);
			_parse(str);
		}
	};

}
