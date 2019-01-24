#pragma once 

class SocketInit
{
public:


	SocketInit();

	// NOTE: WSACleanup을 호출하는 시점이 있어야 하는 것이 일반적입니다.
	// 그러나, C++에서 전역 객체의 파괴자 호출의 순서를 
	// 알 수 없으므로, 어차피 프로세스를 종료하는 마당이므로 굳이 WSACleanup을 호출하지는 않습니다.
	void Touch();
};

extern SocketInit g_socketInit;