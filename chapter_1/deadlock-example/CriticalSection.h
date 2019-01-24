#pragma once

class CriticalSection
{
	CRITICAL_SECTION m_critSec;
public:
	CriticalSection();
	~CriticalSection();

	void Lock();
	void Unlock();
};

class CriticalSectionLock
{
	CriticalSection* m_pCritSec;
public:
	CriticalSectionLock(CriticalSection& critSec);
	~CriticalSectionLock();
};
