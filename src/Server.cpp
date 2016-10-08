#include <iostream>
#include <string>
#include <stdio.h>

#include "httpdrest/serverPort.hpp"
#include "httpdrest/service.hpp"
#include "httpdrest/method.hpp"

#define PORT 10000

using namespace std;

void exchange(Request&req, Response&res) {
	string messg("hello subworld");
	cout << "hello subworld" << endl;
	res.send(messg);
}

int main(int argc, char**argv)
{
	ServerPort server;
	
	server.addPath(
		Method::create()
		->setName("Exchange")
		->setPath("/exchange")
		->setMethod(METHOD_POST)
		->setAction(&exchange)
	);

	server.start(PORT);

	getchar();

	server.stop(PORT);
	return 0;
}
