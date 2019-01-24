#include "stdafx.h"
#include "Poll.h"

// fdArray와 fdArrayLength가 가리키는 배열의 소켓 중 하나 이상이 이벤트를 일으킬 때까지 기다립니다.
// timeOutMs는 최대 대기 시간을 밀리초 단위로 기다리는 값입니다.
int Poll(PollFD* fdArray, int fdArrayLength, int timeOutMs)
{
	static_assert(sizeof(fdArray[0]) == sizeof(fdArray[0].m_pollfd), ""); // 이걸 만족 못하면 PollFD에 virtual 함수 등 다른 멤버가 들어갔다는 뜻이다. 이러한 경우 PollFD로부터 네이티브 배열로의 복사를 해주는 것을 별도로 구현해야 한다.
#ifdef _WIN32
	return ::WSAPoll((WSAPOLLFD*)fdArray, fdArrayLength, timeOutMs);
#else
	return ::poll((pollfd*)fdArray, fdArrayLength, timeOutMs);
#endif
}
