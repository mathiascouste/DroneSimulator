#ifndef _METHOD_HPP_
#define _METHOD_HPP_

#include <string>
#include <vector>

#include "path.hpp"
#include "request.hpp"
#include "response.hpp"

#define PARAM_NC 0
#define PARAM_PATH 1

class Method : public Path
{
private:
	class Param
	{
	public:
		std::string pName;
		std::string pRegex;
		Param(std::string pName, std::string pRegex): pName(pName), pRegex(pRegex){}
	};
	std::vector<Param> params;
	unsigned int paramType;
	std::string pathRegex;
	
	Method();
	virtual bool acceptUrl(Request&);
public:
	~Method();
	Method* setPath(std::string);
	Method* setName(std::string);
	Method* setMethod(std::string);
	Method* setAction(void(*)(Request&, Response&));
	static Method* create();
	virtual void handle(Request&, Response&);
	Method* addParam(std::string, std::string = "[^/]+");
	Method* setParamType(unsigned int);
	Method* generatePathRegex();
	void build();
};

#endif
