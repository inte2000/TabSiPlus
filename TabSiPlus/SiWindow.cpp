// SiWindow.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/

#include "stdafx.h"
#include "tabsiplus.h"
#include "SiWindow.h"
#include "SIFrameWnd.h"
#include "SiMDIWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SWP_NOEVERYTHING         (SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE)

/////////////////////////////////////////////////////////////////////////////
// CSiWindow

CSiWindow::CSiWindow()
{
    m_szTitle[0] = 0;
}

CSiWindow::~CSiWindow()
{
}

void CSiWindow::Attach(HWND hWnd)
{
    ASSERT(hWnd);
    Subclass(hWnd);
}

HWND CSiWindow::Detach()
{
    HWND hTmp = m_hWnd;
    Unsubclass();
    return hTmp;
}

void CSiWindow::SetActive(BOOL bAvtive)
{
    if(bAvtive)
    {
        ::PostMessage(::GetParent(m_hWnd), WM_MDIACTIVATE,(WPARAM)m_hWnd, 0);
    }
    else
    {
    }
}

void CSiWindow::OnDestroy() 
{
}

void CSiWindow::PostNcDestroy() 
{
}

void CSiWindow::PreSubclassWindow() 
{
    GetWindowText(m_szTitle, 256);
    CSubclassWnd::PreSubclassWindow();
}

/*
一个WM_MDIACTIVATE事件发生时，相关的两个子窗口都会收到WM_MDIACTIVATE消息
但是hActived和hDeactived参数会不同，我们只关心被激活的窗口
*/
void CSiWindow::OnMDIActive(WPARAM wParam, LPARAM lParam)
{
    HWND hDeactived = (HWND)wParam;
    HWND hActived = (HWND)lParam;
    DebugTracing(gnDbgLevelNormalDebug,_T("CSiWindow::OnMDIActive(m_hWnd = %x) hDeactive = %x, hActive = %x"),(int)m_hWnd,(int)hDeactived,(int)hActived);
    
    if(g_pSiFrameWnd && (m_hWnd == hActived))
    {
        g_pSiFrameWnd->OnSiWindowActivated(hActived, hDeactived);
    }

}

int CSiWindow::OnGetWindowText(WPARAM wParam, LPARAM lParam)
{
    if(lParam)
    {
        if(lstrcmp(m_szTitle,(LPTSTR)lParam) != 0)//标题发生改变了
        {
            lstrcpy(m_szTitle,(LPTSTR)lParam);
            if(g_pSiFrameWnd)
            {
                DebugTracing(gnDbgLevelNormalDebug,_T("CSiWindow::OnGetWindowText(%s)"),m_szTitle);
                g_pSiFrameWnd->OnSiWindowTitleChanged(m_hWnd, m_szTitle);
            }
        }
    }

    return 0;
}

LRESULT CSiWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lr;
    switch(uMsg)
    {
        case WM_GETTEXT:
        {
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            OnGetWindowText(wParam,lParam);
            return lr;
        }
        case WM_DESTROY:
        {
            OnDestroy();
            break;//让默认的处理函数继续处理
        }
        case WM_MDIACTIVATE:
        {
            OnMDIActive(wParam,lParam);
            break;//让默认的处理函数继续处理
        }
#ifdef INSTALL_SI_MENU_THEME
        case WM_UNINITMENUPOPUP://WM_EXITMENULOOP:
        {
            m_ActiveMenus.OnUninitMenuPopup(wParam, lParam);
            break;
        }
        case WM_INITMENUPOPUP/*WM_INITMENU*/:
        {
            m_ActiveMenus.OnInitMenuPopup(wParam, lParam);
            break;
        }
        case WM_MEASUREITEM:
        {
            int nRtn = m_ActiveMenus.OnMeasureItem(wParam, lParam);
            if(nRtn == 1)
                return 1;
            else
                return CSubclassWnd::WindowProc(uMsg, wParam, lParam);
        }
        case WM_DRAWITEM:
        {
            int nRtn = m_ActiveMenus.OnDrawItem(wParam, lParam);
            if(nRtn == 1)
                return 1;
            else
                return CSubclassWnd::WindowProc(uMsg, wParam, lParam);
        }
#endif
        default:
            break;
    }

    return CSubclassWnd::WindowProc(uMsg, wParam, lParam);
}