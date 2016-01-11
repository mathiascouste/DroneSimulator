#include "response.hpp"
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

Response::Response()
: download(0), downloadSize(0)
{
	
}

Response::~Response()
{
	if(this->download != NULL) {
		free(this->download);
	}
}


void Response::send(string str)
{
	this->download = malloc(str.size());
	str.copy((char*)this->download, str.size());
	this->downloadSize = str.size();
}
