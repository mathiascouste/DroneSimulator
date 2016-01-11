#include "serverPort.hpp"

#include <string.h>
#include <iostream>
#include <microhttpd.h>
#include <string>

using namespace std;

ServerPort* ServerPort::instance;

ServerPort::ServerPort()
{
	this->name = "serverPort";
}

ServerPort::~ServerPort()
{
}

void ServerPort::start(unsigned short port)
{
	ServerPort::instance = this;
	this->build();

	struct MHD_Daemon * daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, port, NULL, NULL, 
                             &answer_to_connection, NULL, MHD_OPTION_END);
    
    daemonPorts[port] = daemon;
    
	cout << "Server start." << endl << "Listening on port " << port << "." << endl;
}

void ServerPort::stop(unsigned short port)
{
	MHD_stop_daemon (daemonPorts[port]);
	daemonPorts.erase(port);
	cout << "Server stopped listening to port " << port << "." << endl;
}

void ServerPort::test() {
	Request req;
	Response res;
	handle(req,res);
}

int answer_to_connection (void *cls, struct MHD_Connection *connection, 
                          const char *url, 
                          const char *method, const char *version, 
                          const char *upload_data, 
                          size_t *upload_data_size, void **con_cls)
{
	cout << "New connection ..." << endl;
	ServerPort * sP = ServerPort::instance;
	
	Request req;
	req.url = string(url);
	req.method = string(method);
	cout << (size_t)(*upload_data_size) << endl;
	req.upload = upload_data;
	req.uploadSize = *upload_data_size;
	Response res;
	
	sP->handle(req,res);
	
	struct MHD_Response *response;
	int ret;
	
	response = MHD_create_response_from_buffer (res.downloadSize, (void*)res.download, MHD_RESPMEM_MUST_COPY);
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

	return ret;
}

static int post_iterator (void *cls,
	       enum MHD_ValueKind kind,
	       const char *key,
	       const char *filename,
	       const char *content_type,
	       const char *transfer_encoding,
	       const char *data, uint64_t off, size_t size)
{
	/*
  struct Request *request = cls;
  struct Session *session = request->session;

  if (0 == strcmp ("DONE", key))
    {
      fprintf (stdout,
	       "Session `%s' submitted `%s', `%s'\n",
	       session->sid,
	       session->value_1,
	       session->value_2);
      return MHD_YES;
    }
  if (0 == strcmp ("v1", key))
    {
      if (size + off >= sizeof(session->value_1))
	size = sizeof (session->value_1) - off - 1;
      memcpy (&session->value_1[off],
	      data,
	      size);
      session->value_1[size+off] = '\0';
      return MHD_YES;
    }
  if (0 == strcmp ("v2", key))
    {
      if (size + off >= sizeof(session->value_2))
	size = sizeof (session->value_2) - off - 1;
      memcpy (&session->value_2[off],
	      data,
	      size);
      session->value_2[size+off] = '\0';
      return MHD_YES;
    }
  fprintf (stderr,
           "Unsupported form value `%s'\n",
           key);
           * */
  return MHD_YES;
}

int answer_to_connection2 (void *cls, struct MHD_Connection *connection, 
                          const char *url, 
                          const char *method, const char *version, 
                          const char *upload_data, 
                          size_t *upload_data_size, void **ptr)
{
	struct MHD_Response *response;
	Request * request;
	struct Session * session;
	request = (Request*)*ptr;
	if(request == NULL) {
		request = new Request();
		request->url = string(url);
		request->method = string(method);
		*ptr = request;
		
		if(request->method.compare("POST") == 0) {
			request->pp = MHD_create_post_processor (connection, 1024, &post_iterator, request);
			if (NULL == request->pp) {
				cout << "Internal error" << endl;
				return MHD_NO;
			}
		}
		return MHD_YES;
	}
	session = request->session;
	session->start = time (NULL);
	if(request->method.compare("POST") == 0) {
		MHD_post_process (request->pp, upload_data, *upload_data_size);
		if(*upload_data_size != 0) {
			*upload_data_size = 0;
			return MHD_YES;
		}
	}
}
