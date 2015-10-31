// MdiChildManagment.h: interface for the CMdiChildManagment class.
/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/

#if !defined(AFX_MDICHILDMANAGMENT_H__126D32DB_9AFA_4A4A_9F27_AB35DAA0B564__INCLUDED_)
#define AFX_MDICHILDMANAGMENT_H__126D32DB_9AFA_4A4A_9F27_AB35DAA0B564__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSiWindow;
class CSiMDIWnd;

typedef struct tagChildObject
{
    HWND              hWnd;
    CString           strFileName;
    CString           strFullName;
    CSiWindow        *pSiWnd;
    BOOL              bIsFileWindow;
    BOOL              bReadonly;
    BOOL              bModify;
}ChildObject;


class CMdiChildManagment
{
public:
    CMdiChildManagment();
    virtual ~CMdiChildManagment();
    BOOL Initialize(CSiMDIWnd  *pMDIClientWnd);
    void Uninitialize();
    BOOL Refresh();
    int EnumMdiChildWnd();
    HWND FindNewMdiChildWnd();

    ChildObject* LookupMdiChild(HWND hWnd);
    ChildObject* LookupMdiChildByFileName(LPCTSTR fileName);
    void RemoveAllMdiChildWnd();
    BOOL AddMdiChildWnd(HWND hWnd);
    BOOL RemoveMdiChildWnd(HWND hWnd);
    BOOL AttachMdiChild(HWND hActivate, CString& strCaption);
    int GetMdiChildCount();
    ChildObject* GetMdiChild(int index);

protected:
    void ReleaseChildObject(ChildObject *pObject);
    ChildObject* FindChild(HWND hWnd);

protected:
    CSiMDIWnd  *m_pMDIClientWnd;
    CArray<ChildObject *, ChildObject *> m_MdiChild;        // Main control map
    CSleepEx  *m_pEvent;
    bool m_bInit;
};

CString GetAssocFileName(ChildObject *pObject);

#endif // !defined(AFX_MDICHILDMANAGMENT_H__126D32DB_9AFA_4A4A_9F27_AB35DAA0B564__INCLUDED_)
