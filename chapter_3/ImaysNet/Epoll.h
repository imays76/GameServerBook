#pragma once

#include <unistd.h>
#include <sys/epoll.h>

class Socket;
class EpollEvents;

// Linux epoll 객체입니다.
class Epoll
{
public:
	// 1회의 epoll_wait이 최대한 꺼내올 수 있는 일의 갯수
	static const int MaxEventCount = 1000;

	int m_epollFd;

	Epoll();
	~Epoll();

	void Add(Socket& socket, void* userPtr, int eventFlags);
	void Wait(EpollEvents& output, int timeoutMs);
};

// epoll_wait으로 꺼내온 이벤트들
class EpollEvents
{
public:
	// epoll_wait으로 꺼내온 이벤트들
	epoll_event m_events[Epoll::MaxEventCount];
	int m_eventCount;
};


