#include "Server.hpp"

int main(int argc, char* argv[]) throw (int)
{
	test::Server http;
	http.setRoute("/", [](test::Request& request, test::Response& response)->void {
		response.setField("Context-Type", "text/html");
		response.setBody("<HTML>Hello world</HTML>");
		});

	http.listen(8899);
	http.start();
}
