#include "method.hpp"

#include <regex>
#include <sstream>

using namespace std;

Method::Method()
:paramType(PARAM_PATH)
{
}

Method::~Method()
{
}

Method* Method::create()
{
	return new Method();
}

Method* Method::setPath(string path)
{
	this->path = path;
	return this;
}

Method* Method::setName(string name)
{
	this->name = name;
	return this;
}

Method* Method::setMethod(string method)
{
	this->method = method;
	return this;
}

Method* Method::setAction(void(*action)(Request&, Response&))
{
	this->action = action;
	return this;
}

void Method::handle(Request& request, Response& response)
{
	if(this->action != NULL) {
		this->action(request, response);
	}
}

Method* Method::addParam(string paramName, string paramRegex)
{
	this->params.push_back(Param(paramName, paramRegex));
	return this;
}

Method* Method::setParamType(unsigned int paramType)
{
	this->paramType = paramType;
	this->generatePathRegex();
	return this;
}

Method* Method::generatePathRegex()
{
	stringstream ss;
	ss << this->path;
	for(unsigned int i = 0 ; i < this->params.size() ; i++) {
		switch(this->paramType) {
			case PARAM_PATH: {
				ss << "/";
				ss << this->params[i].pRegex;
				break;
			}
		}
	}
	this->pathRegex = ss.str();

	return this;
}

void Method::build() {
	this->generatePathRegex();
}

bool Method::acceptUrl(Request& request) {
	if (!regex_match (request.url, regex(this->pathRegex, std::regex_constants::extended) )) {
		return false;
	} else {
		request.url = request.url.substr(this->path.size());
	}
	switch(this->paramType) {
		case PARAM_PATH: {
			for(unsigned int i = 0 ; i < this->params.size() ; i++) {
				request.url = request.url.substr(1);
				smatch m;
				if(regex_search (request.url, m, regex(this->params[i].pRegex, std::regex_constants::extended))) {
					string foundString = m[0];
					request.url = request.url.substr(foundString.size());
					request.parameters[this->params[i].pName] = foundString;
				}
			}
			break;
		}
	}
	return true;
}
