#ifndef _SERVICE_HPP_
#define _SERVICE_HPP_

#include <string>
#include <vector>
#include "path.hpp"
#include "request.hpp"
#include "response.hpp"

class Service : public Path
{
private:
	std::vector<Path*> pathes;
protected:	
	Service();
public:
	~Service();
	Service* setPath(std::string);
	Service* setName(std::string);
	Service* setMethod(std::string);
	Service* setAction(void(*)(Request&, Response&));
	Service* addPath(Path*);
	void build();

	static Service* create();
	virtual void handle(Request&, Response&);
};

#endif
