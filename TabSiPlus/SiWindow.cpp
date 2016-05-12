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

#define PW_TIMER_ID          0x1423   //need a very very very different value that source insight may using
#define PW_TIMER_ELAPSE      25
/////////////////////////////////////////////////////////////////////////////
// CSiWindow

CSiWindow::CSiWindow() : m_powerMode(GlobalPowerModeConfig())
{
    m_szTitle[0] = 0;
    m_timerId = 0;
}

CSiWindow::~CSiWindow()
{
}

void CSiWindow::Attach(HWND hWnd)
{
    ASSERT(hWnd);
    Subclass(hWnd);
    m_timerId = ::SetTimer(hWnd, PW_TIMER_ID, PW_TIMER_ELAPSE, NULL);
    m_powerMode.Attach(hWnd);
}

HWND CSiWindow::Detach()
{
    if(m_timerId != 0)
    {
        ::KillTimer(m_hWnd, PW_TIMER_ID);
        m_timerId = 0;
    }
    m_powerMode.Detach();
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

void CSiWindow::PostNcDestroy() 
{
}

void CSiWindow::PreSubclassWindow() 
{
    GetWindowText(m_szTitle, 256);
    CSubclassWnd::PreSubclassWindow();
}

void CSiWindow::OnDestroy() 
{
}

void CSiWindow::OnTimer(UINT_PTR nIDEvent)
{
    if(m_timerId == nIDEvent)
    {
        m_powerMode.ShowParticles();
    }
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

BOOL IsNeedBurstChar(UINT nChar)
{
    if((nChar >= 0x41) && (nChar <= 0x5A))
        return TRUE;

    if((nChar >= 0x60) && (nChar <= 0x7A))
        return TRUE;

    if((nChar >= 0x30) && (nChar <= 0x39))
        return TRUE;

    if((nChar >= 0x321) && (nChar <= 0x32B))
        return TRUE;

    if((nChar >= 0x33A) && (nChar <= 0x340))
        return TRUE;

    if((nChar >= 0x37B) && (nChar <= 0x37E))
        return TRUE;

    if((nChar >= 0x35E) && (nChar <= 0x35F))
        return TRUE;

    if((nChar >= 0x5B) && (nChar <= 0x5D))
        return TRUE;

    if((nChar >= 0x2C) && (nChar <= 0x2F))
        return TRUE;

    if((nChar == 0x08) || (nChar == 0x09) || (nChar == 0x0D) || (nChar == 0x20) 
        || (nChar == 0x308) || (nChar == 0x309) || (nChar == 0x30D) || (nChar == 0x320) 
        || (nChar == 0x27) || (nChar == 0x802E) || (nChar == 0x832E) || (nChar == 0x3B) 
        || (nChar == 0x3D))
    {
        return TRUE;
    }

    return FALSE;
}

void CSiWindow::OnKeyMessage(UINT nChar, UINT nFlags)
{
    TRACE(_T("0x7e9 nChar = %x, nFlags = %x\n"), nChar, nFlags);

    if(IsNeedBurstChar(nChar))
    {
        POINT carPt = { 0 };
        ::GetCaretPos(&carPt);
        if((carPt.x >= 0) && (carPt.y >= 0))
        {
            //ClientToScreen(&carPt);
            TRACE(_T("carPt.x = %d, carPt.y = %d\n"), carPt.x, carPt.y);
            m_powerMode.AddParticles(carPt.x, carPt.y);
        }
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
    TRACE(_T("Current Thread Id = %x, uMsg = %x\n"), ::GetCurrentThreadId(), uMsg);

    switch(uMsg)
    {
        case WM_GETTEXT:
        {
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            OnGetWindowText(wParam,lParam);
            return lr;
        }
        case 0x7e9:
        {
            UINT nChar = static_cast<UINT>(wParam);
            UINT nFlags = static_cast<UINT>(lParam);

            if((nChar == 0x08) || (nChar == 0x09) || (nChar == 0x0D) || (nChar == 0x802e)
                || (nChar == 0x308) || (nChar == 0x309) || (nChar == 0x30D)) 
            {
                lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
                OnKeyMessage(nChar, nFlags);
            }
            else
            {
                OnKeyMessage(nChar, nFlags);
                lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            }
            
            return lr;
        }
        case WM_TIMER:
        {
            UINT_PTR timerId = static_cast<UINT_PTR>(wParam);
            OnTimer(timerId);
            break;//让默认的处理函数继续处理
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