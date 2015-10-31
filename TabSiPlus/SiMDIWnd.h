/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/
#if !defined(AFX_SIMDIWND_H__ECD29FAC_F325_478A_BA79_C59B5AA6C7A4__INCLUDED_)
#define AFX_SIMDIWND_H__ECD29FAC_F325_478A_BA79_C59B5AA6C7A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SiMDIWnd.h : header file
//
#include "SubclassWnd.h"

class CTabBarsWnd;
/////////////////////////////////////////////////////////////////////////////
// CSiMDIWnd window

class CSiMDIWnd : public CSubclassWnd
{
// Construction
public:
    CSiMDIWnd(HWND hMdiClient);
    virtual ~CSiMDIWnd();

// Attributes
public:
    bool SetManaging(bool bManaging, SnapOrientations tabbarPos);
    bool GetManaging() { return m_bManaging; };
    void SetTabbarWnd(CTabBarsWnd *pTabbarWnd) { m_pTabbarWnd = pTabbarWnd; };

// Operations
public:
    BOOL IsMdiChildWnd(HWND hWnd);
// Implementation
public:
    virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void PreSubclassWindow();

    HWND MDIGetActive(BOOL *pbMaximized = NULL);

    void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    void OnMDIActivate(HWND hActivate);
    void OnTabCtrlRowCountChanged(int tabWndHeight, int rowCount);


protected:
    CTabBarsWnd *m_pTabbarWnd;
     bool  m_bManaging;
    RECT  m_LastPos;
    RECT  m_LastParentPos;
    int   m_iTabHeight;

    void ForceRedraw(SnapOrientations tabbarPos);
    void ReclameTabSpace(SnapOrientations tabbarPos);
    void UpdateTabSizeAndPosition(RECT& WndRect);
};


#endif // !defined(AFX_SIMDIWND_H__ECD29FAC_F325_478A_BA79_C59B5AA6C7A4__INCLUDED_)

