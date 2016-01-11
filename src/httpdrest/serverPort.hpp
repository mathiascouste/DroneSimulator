#ifndef _SERVERPORT_HPP_
#define _SERVERPORT_HPP_

#include <map>
#include <vector>

#include "path.hpp"
#include "service.hpp"

#include <microhttpd.h>

class ServerPort : public Service
{
private:
	std::vector<Path> pathes;
	std::map<unsigned short, struct MHD_Daemon*> daemonPorts;
public:
	ServerPort();
	~ServerPort();
	void start(unsigned short);
	void stop(unsigned short);
	void test();
	
	static ServerPort* instance;
};

int answer_to_connection (void *cls, struct MHD_Connection *connection, 
                          const char *url, 
                          const char *method, const char *version, 
                          const char *upload_data, 
                          size_t *upload_data_size, void **con_cls);



#endif
