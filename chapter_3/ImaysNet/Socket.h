#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <mswsock.h>
#else 
#include <sys/socket.h>
#endif


#include <string>


#ifndef _WIN32
// SOCKET은 64bit 환경에서 64bit이다. 반면 linux에서는 여전히 32bit이다. 이 차이를 위함.
typedef int SOCKET;
#endif

class Endpoint;


enum class SocketType
{
	Tcp,
	Udp,
};

// 소켓 클래스
class Socket
{
public:
	static const int MaxReceiveLength = 8192;


	SOCKET m_fd; // 소켓 핸들

#ifdef _WIN32
	// AcceptEx 함수 포인터
	LPFN_ACCEPTEX AcceptEx = NULL;

	// Overlapped I/O나 IOCP를 쓸 때에만 사용됩니다. 현재 overlapped I/O 중이면 true입니다.
	// (N-send나 N-recv도 가능하게 하려면 이렇게 단일 값만 저장하면 안되지만, 본 소스는 독자의 학습이 우선이므로 이를 고려하지 않았습니다.)
	bool m_isReadOverlapped = false;

	// Overlapped receive or accept을 할 때 사용되는 overlapped 객체입니다. 
	// I/O 완료 전까지는 보존되어야 합니다.
	WSAOVERLAPPED m_readOverlappedStruct;
#endif
	// Receive나 ReceiveOverlapped에 의해 수신되는 데이터가 채워지는 곳입니다.
	// overlapped receive를 하는 동안 여기가 사용됩니다. overlapped I/O가 진행되는 동안 이 값을 건드리지 마세요.
	char m_receiveBuffer[MaxReceiveLength];

#ifdef _WIN32
	// overlapped 수신을 하는 동안 여기에 recv의 flags에 준하는 값이 채워집니다. overlapped I/O가 진행되는 동안 이 값을 건드리지 마세요.
	DWORD m_readFlags = 0;
#endif

	Socket();
	Socket(SOCKET fd);
	Socket(SocketType socketType);
	~Socket();

	void Bind(const Endpoint& endpoint);
	void Connect(const Endpoint& endpoint);
	int Send(const char* data, int length);
	void Close();
	void Listen();
	int Accept(Socket& acceptedSocket, std::string& errorText);
#ifdef _WIN32
	bool AcceptOverlapped(Socket& acceptCandidateSocket, std::string& errorText);
	int UpdateAcceptContext(Socket& listenSocket);
#endif
	Endpoint GetPeerAddr();
	int Receive();
#ifdef _WIN32
	int ReceiveOverlapped();
#endif
	void SetNonblocking();
	
};

std::string GetLastErrorAsString();

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif
