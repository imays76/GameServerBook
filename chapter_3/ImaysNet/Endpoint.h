#pragma once

#ifdef _WIN32
#include <Ws2tcpip.h>
#else 
#include <netinet/in.h>
#include <netinet/ip.h>
#endif

class Endpoint
{
public:
	Endpoint();
	Endpoint(const char* address, int port);
	~Endpoint();

	sockaddr_in m_ipv4Endpoint;

	static Endpoint Any;
	std::string ToString();
};

