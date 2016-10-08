#ifndef _PATH_HPP_
#define _PATH_HPP_

#include <string>

#include "request.hpp"
#include "response.hpp"

#define METHOD_GET "GET"
#define METHOD_POST "POST"

class Path
{
public:
	std::string path;
	std::string name;
	std::string method;
	void (*action)(Request&, Response&);
	Path();
	virtual bool acceptMethod(Request&);
	virtual bool acceptUrl(Request&);
public:
	~Path();
	Path* setPath(std::string);
	Path* setName(std::string);
	Path* setAction(void(*)(Request&, Response&));
	Path* setMethod(std::string);
	std::string getPath();
	std::string getName();
	std::string getMethod();
	virtual void handle(Request&, Response&);
	virtual bool acceptRequest(Request&);
	virtual void build() {};
};

#endif
