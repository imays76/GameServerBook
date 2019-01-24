#include "stdafx.h"
#include "CriticalSection.h"


CriticalSection::CriticalSection()
{
	InitializeCriticalSectionEx(&m_critSec, 0, 0);
}


CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_critSec);
}

void CriticalSection::Lock()
{
	EnterCriticalSection(&m_critSec);
}

void CriticalSection::Unlock()
{
	LeaveCriticalSection(&m_critSec);
}

CriticalSectionLock::CriticalSectionLock(CriticalSection& critSec)
{
	m_pCritSec = &critSec;
	m_pCritSec->Lock();
}

CriticalSectionLock::~CriticalSectionLock()
{
	m_pCritSec->Unlock();
}
