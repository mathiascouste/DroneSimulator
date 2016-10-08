#include "request.hpp"

#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

Request::Request()
:expectedBodySize(-1), actualBodySize(0)
{
}

Request::~Request() {}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void Request::pushFirstLine(std::string line) {
	vector<string> tokens = split(line, ' ');
	method = tokens[0];
	path = tokens[1];
	version = tokens[2];
}

void Request::pushHeaderLine(std::string line) {
	vector<string> tokens = split(line, ':');
	string value = tokens[1];
	for(unsigned int i = 2 ; i < tokens.size() ; i++) {
		value += tokens[i];
	}
	headers[tokens[0]] = value.substr(1);
}

void Request::pushBody(std::string body) {
	cout << body;
	actualBodySize += body.size();
	this->body += body;
}

ostream& operator << (ostream& O, const Request& request) {
	O << request.method << " " << request.path << " " << request.version << endl;
	
	map<string, string> mymap = request.headers;
	typedef std::map<std::string, std::string>::iterator it_type;
	for(it_type it = mymap.begin(); it != mymap.end(); it++) {
		O << it->first << ":" << it->second << endl;
	}
	O << endl << request.body;
	
	return O;
}

void Request::headerIsComplete() {
	if(headers["Content-Length"].size() != 0) {
		expectedBodySize = atoi(headers["Content-Length"].c_str());
	}
}

bool Request::isComplete() {
	return actualBodySize == expectedBodySize;
}
