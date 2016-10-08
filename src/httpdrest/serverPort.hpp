#ifndef _SERVERPORT_HPP_
#define _SERVERPORT_HPP_

// server import

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>

#include <string>
#include <iostream>

#include "request.hpp"

// framework import

#include <map>
#include <vector>

#include "path.hpp"
#include "service.hpp"

class ServerPort : public Service
{
private:
	// socket and server creation and working variables and methods
	int sockfd, new_fd;
	int port;
	struct sigaction sa;
	socklen_t sin_size;
	struct sockaddr_in their_addr;
	bool keepgoing;
	
	void handleChunk(Request & request, char * in, int &body);
	void create_and_bind();

	// framework variable
	std::vector<Path> pathes;
	std::map<unsigned short, struct MHD_Daemon*> daemonPorts;
public:
	ServerPort();
	~ServerPort();
	void start(unsigned short);
	void stop(unsigned short);
	
	static ServerPort* instance;
};

int answer_to_connection (void *cls, struct MHD_Connection *connection, 
                          const char *url, 
                          const char *method, const char *version, 
                          const char *upload_data, 
                          size_t *upload_data_size, void **con_cls);



#endif
