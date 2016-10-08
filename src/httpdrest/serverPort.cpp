#include "serverPort.hpp"

#define MAXREAD 512
#define BACKLOG 10

using namespace std;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

ServerPort* ServerPort::instance;

ServerPort::ServerPort()
{
	this->name = "serverPort";
}

ServerPort::~ServerPort()
{
}

void ServerPort::stop(unsigned short port)
{
    keepgoing = true;
	cout << "Server stopped listening to port " << port << "." << endl;
}

void ServerPort::create_and_bind() {
    int yes=1;
    struct sockaddr_in my_addr;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void ServerPort::handleChunk(Request & request, char * in, int & body) {
	if(!body) {
		char *token;
		string tokenStr;
		const char s[2] = "\n";
	   
		/* get the first token */
		token = strtok(in, s);
		tokenStr = string(token);
		request.pushFirstLine(tokenStr);
					
		/* walk through other tokens */
		cout << body << endl;
		while((token = strtok(NULL, s)) != NULL ) {
			tokenStr = string(token);
			if(!body) {
				tokenStr = tokenStr.erase(tokenStr.find("\r"),1);
			}
			
			if(!body) {
				if(tokenStr.size() != 0) {
					request.pushHeaderLine(tokenStr);
				} else {
					request.headerIsComplete();
					body = 1;
				}
			} else {
				if(!body){
					body=1;
					request.headerIsComplete();
				} else {
					if(body > 1) {
						request.pushBody("\n");
					}
					body=2;
					if(tokenStr.size() > 0) {
						request.pushBody(tokenStr);
					}
				}
			}
		}
	} else {
		request.pushBody(in);
	}
	
	if(request.isComplete()) {
		body = 3;
	}
}

void ServerPort::start(unsigned short port)
{
    char in[MAXREAD];
    this->port = port;
    keepgoing = true;
    create_and_bind();

    //Important stuff happens here.

    while(keepgoing) {
        sin_size = sizeof(struct sockaddr_in);
        printf("listening ...\n");
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

		if (!fork()) {
            close(sockfd);
            Request request;
            Response response;
            
			int body = 0;
			
            while(body != 3 && read(new_fd, in, MAXREAD) > 0) {				
				handleChunk(request, in, body);
			}
			
			cout << "Try to handle this request : " << endl;
			cout << request << endl;
			handle(request, response);
			
			close(new_fd);
			exit(0);
		}
		close(new_fd);
	}
}
