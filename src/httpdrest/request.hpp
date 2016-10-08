#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

#include <string>
#include <microhttpd.h>
#include <map>


class Request
{
private:
	int expectedBodySize;
	int actualBodySize;
public:

	std::map<std::string, std::string > headers;
	std::string method;
	std::string path;
	std::string version;
	std::string body;

	Request();
	~Request();
	std::map<std::string, std::string> parameters;
	
	void pushFirstLine(std::string);
	void pushHeaderLine(std::string);
	void pushBody(std::string);
	bool isComplete();
	void headerIsComplete();
	
	friend std::ostream& operator << (std::ostream& O, const Request& request);
};

#endif
