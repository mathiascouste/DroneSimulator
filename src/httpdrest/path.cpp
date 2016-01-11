#include "path.hpp"

using namespace std;

Path::Path()
:method(METHOD_GET)
{
}

Path::~Path()
{
}

Path* Path::setPath(string path)
{
	this->path = path;
	return this;
}

Path* Path::setName(string name)
{
	this->name = name;
	return this;
}

Path* Path::setAction(void(*action)(Request&, Response&))
{
	this->action = action;
	return this;
}

Path* Path::setMethod(string method)
{
	this->method = method;
	return this;
}

void Path::handle(Request& request, Response& response)
{
	if(this->action != NULL) {
		this->action(request, response);
	}
}

bool Path::accept(Request& request)
{
	return this->acceptMethod(request) && this->acceptUrl(request);
}

bool Path::acceptMethod(Request& request)
{
	if(request.method.compare(this->method)) {
		return false;
	}
	return true;
}

bool Path::acceptUrl(Request& request)
{
	size_t pos = request.url.find(this->path);
	if(pos != 0) {
		return false;
	} else {
		request.url = request.url.substr(this->path.size());
	}
	return true;
}


string Path::getPath()
{
	return this->path;
}

string Path::getName()
{
	return this->name;
}

string Path::getMethod()
{
	return this->method;
}
