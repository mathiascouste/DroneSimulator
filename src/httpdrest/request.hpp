#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

#include <string>
#include <microhttpd.h>
#include <map>

struct Session
{
  /**
   * We keep all sessions in a linked list.
   */
  struct Session *next;

  /**
   * Unique ID for this session.
   */
  char sid[33];

  /**
   * Reference counter giving the number of connections
   * currently using this session.
   */
  unsigned int rc;

  /**
   * Time when this session was last active.
   */
  time_t start;

  /**
   * String submitted via form.
   */
  char value_1[64];

  /**
   * Another value submitted via form.
   */
  char value_2[64];

};

class Request
{
private:
public:
	Request();
	~Request();
	struct Session * session;
	std::string url;
	std::string method;
	const char * upload;
	size_t uploadSize;
	struct MHD_PostProcessor *pp;
	std::map<std::string, std::string> parameters;
};

#endif
