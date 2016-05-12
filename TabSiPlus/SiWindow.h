/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/
#if !defined(AFX_SIWINDOW_H__7AD1F548_1040_4846_8ED8_0801736403F7__INCLUDED_)
#define AFX_SIWINDOW_H__7AD1F548_1040_4846_8ED8_0801736403F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SiWindow.h : header file
//
#include "SubclassWnd.h"
#ifdef INSTALL_SI_MENU_THEME
#include "OwnerDrawMenuMgmt.h"
#endif
#include "PowerMode.h"

/////////////////////////////////////////////////////////////////////////////
// CSiWindow window

class CSiWindow : public CSubclassWnd
{
// Construction
public:
    CSiWindow();
    virtual ~CSiWindow();
// Attributes
public:
    void Attach(HWND hWnd);
    HWND Detach();
    void SetActive(BOOL bAvtive = TRUE);
    TCHAR m_szTitle[256];
// Operations
public:

// Implementation
public:
    virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    virtual void PostNcDestroy();
    virtual void PreSubclassWindow();
    void OnDestroy();
    void OnTimer(UINT_PTR nIDEvent);
    void OnMDIActive(WPARAM wParam, LPARAM lParam);
    void OnKeyMessage(UINT nChar, UINT nFlags);
    int OnGetWindowText(WPARAM wParam, LPARAM lParam);

protected:
    CPowerMode m_powerMode;
    UINT_PTR m_timerId;
#ifdef INSTALL_SI_MENU_THEME
    COwnerDrawMenuMgmt m_ActiveMenus;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIWINDOW_H__7AD1F548_1040_4846_8ED8_0801736403F7__INCLUDED_)
