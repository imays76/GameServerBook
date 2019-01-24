#pragma once 

#ifndef _WIN32
#include <poll.h>
#endif

// unix의 poll, windows의 WSAPoll을 위한 구조체입니다.
class PollFD
{
public:
#ifdef _WIN32
	WSAPOLLFD m_pollfd;
#else
	pollfd m_pollfd;
#endif
};

int Poll(PollFD* fdArray, int fdArrayLength, int timeOutMs);