#include "stdafx.h"
#include "SyncObject.h"



CEventEx::CEventEx(BOOL bManul, BOOL bInitial)
{
    m_hEvent = ::CreateEvent(NULL, bManul, bInitial, NULL);
}

CEventEx::~CEventEx()
{
    ::CloseHandle(m_hEvent);
}

void CEventEx::Lock()
{
    WaitForSingleObject(m_hEvent,INFINITE);
}

void CEventEx::UnLock()
{
    ::SetEvent(m_hEvent);
}


CSleepEx::CSleepEx(BOOL bInitial)
{
    ::InterlockedExchange(&m_lLockCount, bInitial ? 1 : 0);
}

CSleepEx::~CSleepEx()
{
}

void CSleepEx::Lock()
{
    int nCount = 0;
    LONG lInitVal;
    do
    {
        lInitVal = InterlockedCompareExchange(&m_lLockCount,0,1);
        if(lInitVal == 0)
        {
            ::Sleep(10);
            nCount++;
            if(nCount >= 500) /*5�볬ʱ*/
            {
                m_lLockCount = 1;
                break;
            }
        }
    }while(lInitVal == 0);
}

void CSleepEx::UnLock()
{
    ::InterlockedExchange(&m_lLockCount,1);
}




CEventLock::CEventLock(CSyncObjectEx *pObj, BOOL bInitLock)
{
    m_pSyncObj = pObj;
    if(bInitLock)
        m_pSyncObj->Lock();
}

CEventLock::~CEventLock()
{
    ASSERT(m_pSyncObj);
    m_pSyncObj->UnLock();
}

void CEventLock::Lock()
{
    ASSERT(m_pSyncObj);
    m_pSyncObj->Lock();
}

void CEventLock::Unlock()
{
    ASSERT(m_pSyncObj);
    m_pSyncObj->UnLock();
}
