#include "stdafx.h"
#include "Endpoint.h"


Endpoint::Endpoint()
{
	memset(&m_ipv4Endpoint, 0, sizeof(m_ipv4Endpoint));
	m_ipv4Endpoint.sin_family = AF_INET;

}

// address, port를 입력받아 endpoint를 준비한다.
Endpoint::Endpoint(const char* address, int port)
{
	memset(&m_ipv4Endpoint, 0, sizeof(m_ipv4Endpoint));
	m_ipv4Endpoint.sin_family = AF_INET;
	inet_pton(AF_INET, address, &m_ipv4Endpoint.sin_addr);
	m_ipv4Endpoint.sin_port = htons((uint16_t)port); 
}

Endpoint::~Endpoint()
{
}

Endpoint Endpoint::Any;

std::string Endpoint::ToString()
{
	char addrString[1000];
	addrString[0] = 0;
	inet_ntop(AF_INET, &m_ipv4Endpoint.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[1000];
	sprintf(finalString, "%s:%d", addrString, htons(m_ipv4Endpoint.sin_port));

	return finalString;
}
