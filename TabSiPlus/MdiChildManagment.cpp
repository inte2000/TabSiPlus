// MdiChildManagment.cpp: implementation of the CMdiChildManagment class.
/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/
#include "stdafx.h"
#include "tabsiplus.h"
#include "SiWindow.h"
#include "SiMDIWnd.h"
#include "MdiChildManagment.h"
#include "Globals.h"
#include "SourceInsight.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMdiChildManagment::CMdiChildManagment() : m_bInit(false), m_pMDIClientWnd(NULL)
{
}

CMdiChildManagment::~CMdiChildManagment()
{
    Uninitialize();
}

BOOL CMdiChildManagment::Initialize(CSiMDIWnd  *pMDIClientWnd)
{
    ASSERT(!m_bInit);

    m_pEvent = new CSleepEx(TRUE);
    m_bInit = true;
    m_pMDIClientWnd = pMDIClientWnd;

    return TRUE;
}

void CMdiChildManagment::Uninitialize()
{
    if(m_bInit)
    {
        RemoveAllMdiChildWnd();
        delete m_pEvent;
        m_bInit = false;
    }
}

int CMdiChildManagment::EnumMdiChildWnd()
{
    ASSERT(m_bInit);
    ASSERT(m_pMDIClientWnd != NULL);

    TCHAR cClassName[256];
    int nEnumCount = 0;

    HWND hChildWnd = ::GetTopWindow(m_pMDIClientWnd->GetSafeHwnd());
    while(hChildWnd != NULL)
    {
        ::GetClassName(hChildWnd, cClassName, sizeof(cClassName));
        if(lstrcmp(cClassName, lpszChindFrameWndClass) == 0)
        {
            if(AddMdiChildWnd(hChildWnd))//如果已经存在不会重复添加
                nEnumCount++;
        }
        hChildWnd = ::GetNextWindow(hChildWnd, GW_HWNDNEXT);
    }

    return nEnumCount;
}

HWND CMdiChildManagment::FindNewMdiChildWnd()
{
    ASSERT(m_bInit);
    ASSERT(m_pMDIClientWnd != NULL);

    CEventLock lock(m_pEvent);

    TCHAR cClassName[256];
    HWND hNewWnd = NULL;

    HWND hChildWnd = m_pMDIClientWnd->GetTopWindow();
    while(hChildWnd != NULL)
    {
        ::GetClassName(hChildWnd, cClassName, sizeof(cClassName));
        if(lstrcmp(cClassName, lpszChindFrameWndClass) == 0)
        {
            if(FindChild(hChildWnd) != NULL)//如果已经存在不会重复添加
            {
                hNewWnd = hChildWnd;
                break;
            }
        }
        hChildWnd = ::GetNextWindow(hChildWnd, GW_HWNDNEXT);
    }

    return hNewWnd;
}

BOOL CMdiChildManagment::Refresh()
{
    ASSERT(m_bInit);
    ASSERT(m_pMDIClientWnd != NULL);
    
    RemoveAllMdiChildWnd();

    EnumMdiChildWnd();

    return TRUE;
}

ChildObject* CMdiChildManagment::LookupMdiChild(HWND hWnd)
{
    ASSERT(m_bInit);
    CEventLock lock(m_pEvent);

    ChildObject *pObj = FindChild(hWnd);

    return pObj;
}

ChildObject* CMdiChildManagment::LookupMdiChildByFileName(LPCTSTR fileName)
{
    ASSERT(m_bInit);
    CEventLock lock(m_pEvent);

    int count = m_MdiChild.GetSize();
    for(int i = 0; i < count; ++i)
    {
        if(m_MdiChild[i]->bIsFileWindow && (m_MdiChild[i]->strFullName.CollateNoCase(fileName) == 0))
        {
            return m_MdiChild[i];
        }
    }

    return NULL;
}

BOOL CMdiChildManagment::RemoveMdiChildWnd(HWND hWnd)
{
    ASSERT(m_bInit);
    CEventLock lock(m_pEvent);

    BOOL bFind = FALSE;
    int count = m_MdiChild.GetSize();
    for(int i = 0; i < count; ++i)
    {
        ChildObject *pObject = m_MdiChild[i];
        ASSERT(pObject);
        if(pObject->hWnd == hWnd)
        {
            ReleaseChildObject(pObject);
            m_MdiChild.RemoveAt(i);
            bFind = TRUE;
            break;
        }
    }

    return bFind;
}

BOOL CMdiChildManagment::AddMdiChildWnd(HWND hWnd)
{
    ASSERT(m_bInit);
    ASSERT(m_pMDIClientWnd != NULL);

    CEventLock lock(m_pEvent);

    ChildObject *pObj = FindChild(hWnd);
    if(pObj != NULL)
    {
        return TRUE;
    }
    if(!m_pMDIClientWnd->IsMdiChildWnd(hWnd))
    {
        return FALSE;
    }

    BOOL bSuccess = FALSE;
    ChildObject *pNewObj = new ChildObject;
    if(pNewObj)
    {
        pNewObj->hWnd    = hWnd;
        pNewObj->bIsFileWindow = GetFileNameFromWindowTitle(hWnd, pNewObj->strFileName, pNewObj->bReadonly, pNewObj->bModify);
        pNewObj->pSiWnd = new CSiWindow();
        if(pNewObj->hWnd != NULL)
        {
            pNewObj->pSiWnd->Attach(pNewObj->hWnd);
        }
        m_MdiChild.Add(pNewObj);
        bSuccess = TRUE;
    }

    return bSuccess;
}

void CMdiChildManagment::RemoveAllMdiChildWnd()
{
    ASSERT(m_bInit);
    CEventLock lock(m_pEvent);

    int count = m_MdiChild.GetSize();
    for(int i = 0; i < count; ++i)
    {
        ChildObject *pObject = m_MdiChild[i];
        ReleaseChildObject(pObject);
    }

    m_MdiChild.RemoveAll();
}

int CMdiChildManagment::GetMdiChildCount()
{
    ASSERT(m_bInit);
    CEventLock lock(m_pEvent);
    return m_MdiChild.GetSize(); 
}

ChildObject* CMdiChildManagment::GetMdiChild(int index)
{
    ASSERT(m_bInit);
    CEventLock lock(m_pEvent);
    ASSERT((index >= 0) && (index < (int)m_MdiChild.GetSize()));

    return m_MdiChild[index];
}

ChildObject* CMdiChildManagment::FindChild(HWND hWnd)
{
    int count = m_MdiChild.GetSize();
    for(int i = 0; i < count; ++i)
    {
        if(m_MdiChild[i]->hWnd == hWnd)
        {
            return m_MdiChild[i];
        }
    }

    return NULL;
}

void CMdiChildManagment::ReleaseChildObject(ChildObject *pObject)
{
    ASSERT(pObject != NULL);
    pObject->pSiWnd->Detach();
    delete pObject->pSiWnd;
    delete pObject;
}

CString GetAssocFileName(ChildObject *pObject)
{
    CString strFullName;
    if(pObject && pObject->pSiWnd)
    {
        BOOL bSuccess = MyGetFullPathName(pObject->strFileName, strFullName.GetBuffer(MAX_PATH), MAX_PATH);
        strFullName.ReleaseBuffer();
        if(!bSuccess)
            strFullName.Empty();
    }

    return strFullName;
}


