#ifdef _WIN32
#include "../ImaysNet/ImaysNet.h"
#else
#include "../ImaysNetLinux/ImaysNet.h"

#endif
#include <stdio.h>
#include <signal.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>
#include <mutex>




using namespace std;

// true가 되면 프로그램을 종료합니다.
volatile bool stopWorking = false;


void ProcessSignalAction(int sig_number)
{
	if (sig_number == SIGINT)
		stopWorking = true;
}


int main()
{
	// 사용자가 ctl-c를 누르면 메인루프를 종료하게 만듭니다.
	signal(SIGINT, ProcessSignalAction);

	try
	{
		// epoll로 많은 수의 클라이언트를 받아 처리한다.

		// 많은 수의 클라가 tcp 5555로 연결 들어오고,
		// 그들은 hello world를 열심히 보낼 것이다.
		// 그것을 그냥 에코해주도록 하자.
		// 서버에서는 총 처리한 바이트수를 지속적으로 출력만 하자.
		
		// 성능보다는 소스 가독성을 우선으로 만들어져 있습니다. 
		// 가령 string 객체의 로컬변수 생성,삭제가 잦는 등의
		// 성능상 문제되는 부분은 알아서 개선하고 싶으시면 개선하십시오.

		// TCP 연결 각각의 객체.
		struct RemoteClient
		{
			Socket tcpConnection;		// accept한 TCP 연결
		};
		unordered_map<RemoteClient*, shared_ptr<RemoteClient>> remoteClients;

		// epoll를 준비한다.
		Epoll epoll;

		// TCP 연결을 받는 소켓
		Socket listenSocket(SocketType::Tcp);
		listenSocket.Bind(Endpoint("0.0.0.0", 5555));

		listenSocket.SetNonblocking();
		listenSocket.Listen();

		// epoll에 추가한다.
		epoll.Add(listenSocket, 
			nullptr,			/*리슨소켓은 딱시 RemoteClient가 없으므로*/
			EPOLLIN | EPOLLET);

		cout << "서버가 시작되었습니다.\n";
		cout << "CTL-C키를 누르면 프로그램을 종료합니다.\n";

		// 리슨 소켓과 TCP 연결 소켓 모두에 대해서 I/O 가능(avail) 이벤트가 있을 때까지 기다린다. 
		// 그리고 나서 I/O 가능 소켓에 대해서 일을 한다.

		while (!stopWorking)
		{
			// I/O 가능 이벤트가 있을 때까지 기다립니다.
			EpollEvents readEvents;
			epoll.Wait(readEvents, 100/*타임아웃*/);

			// 받은 이벤트 각각을 처리합니다.
			for (int i = 0; i < readEvents.m_eventCount; i++)
			{
				auto& readEvent = readEvents.m_events[i];
				if (readEvent.data.ptr == 0) // 리슨소켓이면
				{
					// 엣지트리거이므로 더 이상 일을 할 수 없을 때까지 반복해야 합니다.
					// 자세한 것은 책의 3.8절를 참고하세요.
					while (true)
					{
						// accept을 처리한다.
						auto remoteClient = make_shared<RemoteClient>();

						// 이미 "클라이언트 연결이 들어왔음" 이벤트가 온 상태이므로 그냥 이것을 호출해도 된다.
						string ignore;
						if (listenSocket.Accept(remoteClient->tcpConnection, ignore) != 0)
						{
							break; // would block 혹은 여타 에러이면 반복을 그만한다.
						}

						remoteClient->tcpConnection.SetNonblocking();

						// epoll에 추가한다.
						epoll.Add(remoteClient->tcpConnection, remoteClient.get(), EPOLLIN | EPOLLET);

						// 새 클라이언트를 목록에 추가.
						remoteClients.insert({ remoteClient.get(), remoteClient });

						cout << "Client joined. There are " << remoteClients.size() << " connections.\n";
					}
				}
				else  // TCP 연결 소켓이면
				{
					// 받은 데이터를 그대로 회신한다.
					shared_ptr<RemoteClient> remoteClient = remoteClients[(RemoteClient*)readEvent.data.ptr];
					if(remoteClient)
					{
						// 엣지트리거이므로 더 이상 일을 할 수 없을 때까지 반복해야 합니다.
						// 자세한 것은 책의 3.8절를 참고하세요.
						while (true)
						{
							string data;
							int ec = remoteClient->tcpConnection.Receive();
							
							if (ec < 0 && errno == EWOULDBLOCK)
							{
								// would block이면 그냥 반복을 중단.
								break;
							}
							
							if (ec <= 0)
							{
								// 에러 혹은 소켓 종료이다.
								// 해당 소켓은 제거해버리자. 
								remoteClient->tcpConnection.Close();
								remoteClients.erase(remoteClient.get());

								cout << "Client left. There are " << remoteClients.size() << " connections.\n";
								break;
							}
							else
							{
								// 원칙대로라면 TCP 스트림 특성상 일부만 송신하고 리턴하는 경우도 고려해야 하나,
								// 지금은 독자의 이해가 우선이므로, 생략하도록 한다.
								remoteClient->tcpConnection.Send(remoteClient->tcpConnection.m_receiveBuffer, ec);
							}
						}
					}
				}
			}
		}

		// 사용자가 CTL-C를 눌러서 루프를 나갔다. 모든 종료를 하자.
		listenSocket.Close();
		remoteClients.clear();
	}
	catch (Exception& e)
	{
		cout << "Exception! " << e.what() << endl;
	}

	return 0;
}

