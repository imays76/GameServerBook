#include "stdafx.h"
#include "Epoll.h"
#include "Exception.h"
#include "Socket.h"

Epoll::Epoll()
{
	m_epollFd = epoll_create(MaxEventCount);
}

Epoll::~Epoll()
{
	close(m_epollFd);
}

// eventFlags를 EPOLLIN | EPOLLET으로 설정하면, edge trigger & for read 가 된다.
void Epoll::Add(Socket& socket, void* userPtr, int eventFlags)
{
	struct epoll_event e;
	e.events = eventFlags;
	e.data.ptr = userPtr;
	int ret = ::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, socket.m_fd, &e);
	if (ret != 0)
	{
		throw Exception("Epoll.Add failed!");
	}
}

void Epoll::Wait(EpollEvents& output, int timeoutMs)
{
	output.m_eventCount = epoll_wait(m_epollFd, output.m_events, MaxEventCount, timeoutMs);
}
