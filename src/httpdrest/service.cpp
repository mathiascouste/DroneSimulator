#include "service.hpp"
#include <iostream>
using namespace std;

Service::Service()
{
}

Service::~Service()
{
}

Service* Service::addPath(Path *method)
{
	this->pathes.push_back(method);
	return this;
}

Service* Service::create() {
	return new Service();
}

Service* Service::setPath(string path)
{
	this->path = path;
	return this;
}

Service* Service::setMethod(string method)
{
	this->method = method;
	return this;
}

Service* Service::setName(string name)
{
	this->name = name;
	return this;
}

Service* Service::setAction(void(*action)(Request&, Response&))
{
	this->action = action;
	return this;
}

void Service::handle(Request& req, Response& res) {
	for(unsigned int i = 0 ; i < this->pathes.size() ; i++) {
		if(this->pathes[i]->acceptRequest(req)) {
			this->pathes[i]->handle(req, res);
			i = this->pathes.size();
		}
	}
}

void Service::build() {
	for(unsigned int i = 0 ; i < this->pathes.size() ; i++) {
		this->pathes[i]->build();
	}
}
