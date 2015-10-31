// SiMDIWnd.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/

#include "stdafx.h"
#include "tabsiplus.h"
#include "SiMDIWnd.h"
#include "SIFrameWnd.h"
#include "TabBarsWnd.h"
#include "MdiChildManagment.h"
#include "Globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSiMDIWnd

CSiMDIWnd::CSiMDIWnd(HWND hMdiClient) : m_pTabbarWnd(NULL), m_bManaging(false)
{
    SetRectEmpty(&m_LastPos);
    SetRectEmpty(&m_LastParentPos);

    m_iTabHeight = 0;
    Subclass(hMdiClient);
}

CSiMDIWnd::~CSiMDIWnd()
{
}

void CSiMDIWnd::PreSubclassWindow() 
{
    ASSERT(m_hWnd);
    CSubclassWnd::PreSubclassWindow();
}

HWND CSiMDIWnd::MDIGetActive(BOOL *pbMaximized)
{
    // MDI client has been created, get active MDI child
    HWND hActiveWnd = (HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, (LPARAM)pbMaximized);
    return hActiveWnd;
}

void CSiMDIWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    DebugTracing(gnDbgLevelHiDebug,_T("CSiMDIWnd::OnWindowPosChanging(x = %d, y = %d, cx = %d, cy = %d),m_bManaging = %d"),lpwndpos->x,lpwndpos->y,lpwndpos->cx,lpwndpos->cy,m_bManaging);
    
    bool bSysCall = true;
    if(m_bManaging)
    {
        RECT WndRect = {lpwndpos->x,lpwndpos->y,lpwndpos->x + lpwndpos->cx,lpwndpos->y + lpwndpos->cy};
        if (lpwndpos->flags & (SWP_NOSIZE | SWP_NOMOVE))
        {
            RECT WndRectOld;
            GetWindowRect(&WndRectOld);
            ::ScreenToClient(GetParent(),&WndRectOld);
            if (lpwndpos->flags & SWP_NOSIZE)
            {
                WndRect.bottom = WndRect.top  + (WndRectOld.bottom - WndRectOld.top);
                WndRect.right  = WndRect.left + (WndRectOld.right - WndRectOld.left);
            }
            if (lpwndpos->flags & SWP_NOMOVE)
            {
                OffsetRect(&WndRect, WndRectOld.left - lpwndpos->x,WndRectOld.top  - lpwndpos->y);
            }
            bSysCall = false;
        }

        if(bSysCall)
        {
            m_LastParentPos.left = lpwndpos->x;
            m_LastParentPos.top  = lpwndpos->y;
            m_LastParentPos.right = m_LastParentPos.left + lpwndpos->cx;
            m_LastParentPos.bottom = m_LastParentPos.top + lpwndpos->cy;

            UpdateTabSizeAndPosition(WndRect);
        }

        ASSERT(WndRect.left >= 0);
        ASSERT(WndRect.top >= 0);
        lpwndpos->x  = WndRect.left;
        lpwndpos->y  = WndRect.top;
        lpwndpos->cx = WndRect.right - WndRect.left;
        lpwndpos->cy = WndRect.bottom - WndRect.top;
    }
}

void CSiMDIWnd::OnMDIActivate(HWND hActivate)
{
    DebugTracing(gnDbgLevelNormalDebug,_T("CSiMDIWnd::OnMDIActivate (hWnd = %x)"), hActivate);

    if(hActivate != NULL)
    {
        g_pSiFrameWnd->OnMdiWindowActivated(hActivate);
    }
}

void CSiMDIWnd::OnTabCtrlRowCountChanged(int tabWndHeight, int rowCount)
{
    DebugTracing(gnDbgLevelHiDebug,_T("CSiMDIWnd::OnTabCtrlRowCountChanged(tabWndHeight = %d, rowCount = %d)"), tabWndHeight, rowCount);

    m_iTabHeight = tabWndHeight;

    SetRectEmpty(&m_LastPos);
    ::SetWindowPos(m_hWnd, HWND_TOP, m_LastParentPos.left, m_LastParentPos.top, 
                   m_LastParentPos.right - m_LastParentPos.left, m_LastParentPos.bottom - m_LastParentPos.top, SWP_NOZORDER);
}

LRESULT CSiMDIWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lr;
    switch(uMsg)
    {
        case WM_WINDOWPOSCHANGING:
        {
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            OnWindowPosChanging((WINDOWPOS *)lParam);
            return lr;
        }
        case WM_MDICREATE:
        {
            DebugTracing(gnDbgLevelNormalDebug,_T("CSiMDIWnd::WM_MDICREATE begin"));
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            DebugTracing(gnDbgLevelNormalDebug,_T("CSiMDIWnd::WM_MDICREATE end (hWnd = %x)"), lr);
            g_pSiFrameWnd->OnSiWindowCreated((HWND)lr);
            return lr;
        }
        case WM_MDIDESTROY:
        {
            DebugTracing(gnDbgLevelNormalDebug,_T("CSiMDIWnd::WM_MDIDESTROY (hWnd = %x)"),(int)wParam);
            HWND hWnd = (HWND)wParam;
            g_pSiFrameWnd->OnSiWindowClosing(hWnd);
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            return lr;
        }
        case WM_MDIACTIVATE:
        {
            DebugTracing(gnDbgLevelNormalDebug,_T("CSiMDIWnd::WM_MDIACTIVATE (hWnd = %x)"),(int)wParam);
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            OnMDIActivate((HWND)wParam);
            return lr;
        }
        default:
            break;
    }

    return CSubclassWnd::WindowProc(uMsg, wParam, lParam);
}

bool CSiMDIWnd::SetManaging(bool bManaging, SnapOrientations tabbarPos)
{
    DebugTracing(gnDbgLevelHiDebug,_T("CSiMDIWnd::SetManaging(bManaging = %d)[m_bManaging = %d]"),bManaging,m_bManaging);

    if (m_hWnd == NULL)
        return m_bManaging;

    if (bManaging == m_bManaging)
        return m_bManaging;

    bool bOldVal = m_bManaging;
    m_bManaging = bManaging;

    SetRectEmpty(&m_LastPos);
    
    // add/remove the tabs
    if (m_bManaging)
    {
        ForceRedraw(tabbarPos);
    }
    else
    {
        ReclameTabSpace(tabbarPos);
    }

    return bOldVal;
}

void CSiMDIWnd::ForceRedraw(SnapOrientations tabbarPos)
{
    // we want to make sure the OnWindowPosChanging handler is called
    RECT WndRect;
    GetWindowRect(&WndRect);
    ::ScreenToClient(GetParent(), &WndRect);
    ::MoveWindow(m_hWnd, &WndRect, TRUE);
}

void CSiMDIWnd::ReclameTabSpace(SnapOrientations tabbarPos)
{
    RECT WndRect;
    GetWindowRect(&WndRect);
    ::ScreenToClient(GetParent(), &WndRect);

    if (tabbarPos == soTop)
    {
        WndRect.top -= m_iTabHeight;
    }
    else
    {
        WndRect.bottom += m_iTabHeight;
    }

    ::MoveWindow(m_hWnd, &WndRect, TRUE);
}

void CSiMDIWnd::UpdateTabSizeAndPosition(RECT& WndRect)
{
    BOOL bPosChanged = !EqualRect(&WndRect,&m_LastPos);
    DebugTracing(gnDbgLevelHiDebug,_T("CSiMDIWnd::UpdateTabSizeAndPosition(WndRect [left = %d, top = %d, right = %d, bottom = %d], bPosChanged = %d)"),
                 WndRect.left,WndRect.top,WndRect.right,WndRect.bottom,bPosChanged);
    m_LastPos = WndRect;
    if(m_pTabbarWnd && m_pTabbarWnd->GetSafeHwnd())
    {
        m_iTabHeight = m_pTabbarWnd->CalcTabbarWndHeight(&WndRect);

        RECT TabsRect = WndRect;
        if (cfg_TabWndPosition == soTop)
        {
            WndRect.top += m_iTabHeight;
            TabsRect.bottom = TabsRect.top + m_iTabHeight;
        }
        else
        {
            WndRect.bottom -= m_iTabHeight;
            TabsRect.top = TabsRect.bottom - m_iTabHeight;
        }
        if(bPosChanged)
        {
            ::ClientToScreen(GetParent(), &TabsRect);
            m_pTabbarWnd->MdiClientPosChanging(&TabsRect);
        }
    }
}

BOOL CSiMDIWnd::IsMdiChildWnd(HWND hWnd)
{
    ASSERT(m_hWnd != NULL);

    ::HWND hParent = ::GetParent(hWnd);

    return (hParent == m_hWnd);
}
