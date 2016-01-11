#ifndef _RESPONSE_HPP_
#define _RESPONSE_HPP_

#include <string>

class Response
{
private:
public:
	void * download;
	unsigned int downloadSize;
	void send(std::string);
	Response();
	~Response();
};

#endif
