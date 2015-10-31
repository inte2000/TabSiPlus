#ifndef __SYNCOBJECT_H__
#define __SYNCOBJECT_H__

class CSyncObjectEx
{
public:
    CSyncObjectEx() 
    {
    }
    virtual ~CSyncObjectEx() 
    {
    }

    virtual void Lock() = 0;
    virtual void UnLock() = 0;
};

class CEventEx : public CSyncObjectEx
{
public:
    CEventEx(BOOL bManul, BOOL bInitial);
    virtual ~CEventEx();

    virtual void Lock();
    virtual void UnLock();

protected:
    HANDLE m_hEvent;
};

class CSleepEx : public CSyncObjectEx
{
public:
    CSleepEx(BOOL bInitial);
    virtual ~CSleepEx();

    virtual void Lock();
    virtual void UnLock();

protected:
    LONG m_lLockCount;
};


class CEventLock
{
public:
    CEventLock(CSyncObjectEx *pObj, BOOL bInitLock = TRUE);
    ~CEventLock();
    void Lock();
    void Unlock();
private:
    CSyncObjectEx *m_pSyncObj;
};



#endif //__SYNCOBJECT_H__