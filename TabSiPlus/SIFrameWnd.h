/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/
#if !defined(AFX_SIFRAMEWND_H__98969861_1BF7_48BD_9357_450778F98CF8__INCLUDED_)
#define AFX_SIFRAMEWND_H__98969861_1BF7_48BD_9357_450778F98CF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIFrameWnd.h : header file
//
#include "SubclassWnd.h"
#ifdef INSTALL_SI_MENU_THEME
#include "OwnerDrawMenuMgmt.h"
#endif
#include "TabBarsWnd.h"

class CSiMDIWnd;
class CMdiChildManagment;
/////////////////////////////////////////////////////////////////////////////
// CSIFrameWnd window

class CSIFrameWnd : public CSubclassWnd
{
// Construction
public:
    CSIFrameWnd(HWND hSiFrameWnd);
    virtual ~CSIFrameWnd();

    BOOL ShowTabbarWnd();
    void CloseTabbarWnd(BOOL bDestoryTab);
    void ShutDownTabSiPlus();
    CTabBarsWnd *GetTabbarWnd() { return &m_TabbarWnd; }
    CSiMDIWnd* GetSiMDIWnd() { return m_pMdiClient; }
    CMdiChildManagment *GetMdiChildMgmt() { return m_pMdiChildMng; }

// Attributes
public:
//    CString m_strTitle;
    void OnSiWindowTitleChanged(HWND hWnd, LPCTSTR lpszTitle);
    void OnSiWindowActivated(HWND hActived, HWND hDeactived);
    void OnMdiWindowActivated(HWND hActived);
    void OnSiWindowCreated(HWND hNewWnd);
    void OnSiWindowClosing(HWND hWnd);

// Operations
protected:
    HWND GetMDIClientWnd();

// Implementation
public:
    virtual void PreSubclassWindow();
    virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
#ifdef INSTALL_SI_MENU_THEME
    COwnerDrawMenuMgmt m_ActiveMenus;
#endif
    CSiMDIWnd *m_pMdiClient;
    CTabBarsWnd  m_TabbarWnd;
    CMdiChildManagment *m_pMdiChildMng;

//    int OnGetWindowText(WPARAM wParam, LPARAM lParam);
};


#endif // !defined(AFX_SIFRAMEWND_H__98969861_1BF7_48BD_9357_450778F98CF8__INCLUDED_)
