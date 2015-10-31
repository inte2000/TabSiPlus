// SIFrameWnd.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : November 22, 2000
*    Last-modified  : July 7, 2003 
*    Contact us     : inte2000@163.com
**********************************************************************/

#include "stdafx.h"
#include "TabSiPlus.h"
#include "SIFrameWnd.h"
#include "SiMDIWnd.h"
#include "TabBarsWnd.h"
#include "SourceInsight.h"
#include "TabWndUIThread.h"
#include "Globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSIFrameWnd *g_pSiFrameWnd = NULL;  // Source insight main window

/////////////////////////////////////////////////////////////////////////////
// CSIFrameWnd
CSIFrameWnd::CSIFrameWnd(HWND hSiFrameWnd)
{
    m_pMdiClient   = NULL;
    m_pMdiChildMng = NULL;
    Subclass(hSiFrameWnd);
}

CSIFrameWnd::~CSIFrameWnd()
{
}

BOOL CSIFrameWnd::ShowTabbarWnd()
{
    HWND hMDIClient = GetMDIClientWnd();
    if(hMDIClient)
    {
        DebugTracing(gnDbgLevelNormalDebug,_T("MDI Client Attach..."));
        m_pMdiClient = new CSiMDIWnd(hMDIClient);
        m_pMdiChildMng = new CMdiChildManagment();
        m_pMdiChildMng->Initialize(m_pMdiClient);
        m_pMdiChildMng->Refresh();

        m_TabbarWnd.CreateEx(WS_EX_NOPARENTNOTIFY, NULL, _T("TabBar"),  WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, 
        0, 0, 0, 0, g_pSiFrameWnd->GetSafeHwnd(), NULL, NULL);

        m_TabbarWnd.SetWindowPos(CWnd::FromHandle(::GetWindow(hMDIClient,GW_HWNDPREV)), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        m_pMdiClient->SetTabbarWnd(&m_TabbarWnd);
        m_pMdiClient->SetManaging(true, cfg_TabWndPosition);

        return TRUE;
    }
    
    return FALSE;
}

void CSIFrameWnd::CloseTabbarWnd(BOOL bDestoryTab)
{
    if(m_pMdiClient)
    {
        m_pMdiClient->SetManaging(false, cfg_TabWndPosition);
        m_pMdiClient->SetTabbarWnd(NULL);
        delete m_pMdiClient;
        m_pMdiClient = NULL;
    }
    if(m_pMdiChildMng)
    {
        m_pMdiChildMng->Uninitialize();
        delete m_pMdiChildMng;
        m_pMdiChildMng = NULL;
    }
    if(bDestoryTab)
    {
        if(m_TabbarWnd.GetSafeHwnd())
        {
            m_TabbarWnd.ShowWindow(SW_HIDE);
            m_TabbarWnd.DestroyWindow();
        }
    }
}

void CSIFrameWnd::ShutDownTabSiPlus()
{
    HANDLE hAnotherTabSiHostEvent = NULL;
    LPCTSTR szGlobalKernelName = _T("Local\\TabSiHostIsAlreadyRunning");
    hAnotherTabSiHostEvent = CreateEvent( NULL, TRUE, FALSE, szGlobalKernelName );    

    DWORD dwer = GetLastError();    

    if(dwer == ERROR_ALREADY_EXISTS)
    {
        ResetEvent( hAnotherTabSiHostEvent );
    }
    ::CloseHandle(hAnotherTabSiHostEvent);
    CloseTabbarWnd(TRUE);
}


HWND CSIFrameWnd::GetMDIClientWnd()
{
    TCHAR cClassName[256];

  HWND hMDIWnd = g_pSiFrameWnd->GetTopWindow();
  ::GetClassName(hMDIWnd, cClassName, sizeof(cClassName));
  while(lstrcmp(cClassName, lpszMdiClientWndClass) != 0)
  {
    hMDIWnd = ::GetNextWindow(hMDIWnd, GW_HWNDNEXT);
    ASSERT(hMDIWnd);
    GetClassName(hMDIWnd, (LPTSTR)cClassName, sizeof(cClassName));
  }

    return hMDIWnd;
}

//@**#---2006-04-17 23:35:29 (oRbIt)---#**@
//int CSIFrameWnd::OnGetWindowText(WPARAM wParam, LPARAM lParam)
//{
//    int nExtraLen = 0;
//    if(lParam)
//    {
//        LPTSTR lpBuffer = (LPTSTR)lParam;
//        lstrcat(lpBuffer,lpszTextMark);
//    }
//
//    return nExtraLen;
//}
//
void CSIFrameWnd::PreSubclassWindow() 
{
    ASSERT(m_hWnd);
    TCHAR szBuffer[384],szTmpBuf[256];
    ::GetWindowText(m_hWnd,szTmpBuf,256);
    //insert our mark to source insight
    LPTSTR lpSiMarkStart = _tcsstr(szTmpBuf, sourceInsightTitle);
    if(lpSiMarkStart)
    {
        LPTSTR lpSiMarkEnd = lpSiMarkStart + _tcslen(sourceInsightTitle);
        int nFirstLen = lpSiMarkEnd - szTmpBuf;
        _tcsncpy(szBuffer,szTmpBuf,nFirstLen);
        szBuffer[nFirstLen] = 0;
        _tcscat(szBuffer,lpszTextMark);
        ::SetWindowText(m_hWnd,szBuffer);
    }
    CSubclassWnd::PreSubclassWindow();
}

LRESULT CSIFrameWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    LRESULT lr;
    switch(uMsg)
    {
        case WM_SETTEXT:
        {
            static TCHAR szBuffer[256];
            if(lParam)
            {
                lstrcpy(szBuffer,(LPCTSTR)lParam);
                lstrcat(szBuffer,lpszTextMark);
                lParam = (LPARAM)szBuffer;
            }
            ::GetCurrentDirectory(MAX_PATH,g_szCurDircetory);
            DebugTracing(gnDbgLevelNormalDebug,_T("SIFrameWnd reach WM_SETTEXT, %s"),g_szCurDircetory);
            break;
        }
        case WM_COMMAND:
        {
//            switch( wParam )
//            {

//            }
            break;
        }
        case WM_DESTROY:
        {
            DebugTracing(gnDbgLevelNormalDebug,_T("SIFrameWnd reach WM_DESTROY, Destroy Tabbar"));
            g_pSiFrameWnd->CloseTabbarWnd(FALSE);
            lr = CSubclassWnd::WindowProc(uMsg, wParam, lParam);
            return lr;
        }
#ifdef INSTALL_SI_MENU_THEME
        case WM_UNINITMENUPOPUP://WM_EXITMENULOOP:
        {
            m_ActiveMenus.OnUninitMenuPopup(wParam, lParam);
            break;
        }
        case WM_INITMENUPOPUP://WM_INITMENU:
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

void CSIFrameWnd::OnSiWindowTitleChanged(HWND hWnd, LPCTSTR lpszTitle)
{
    ASSERT(m_pMdiChildMng != NULL);
    if((hWnd != NULL) && (lpszTitle != NULL))
    {
        ChildObject *pObj = m_pMdiChildMng->LookupMdiChild(hWnd);
        if(pObj)
        {
            pObj->bIsFileWindow = HackFileNameFromTitle(lpszTitle, pObj->strFileName, pObj->bReadonly, pObj->bModify);
            CString strItemTxt = MakeTabItemText(pObj->strFileName, pObj->bReadonly, pObj->bModify);
            m_TabbarWnd.OnSiWindowTitleChanged(hWnd, strItemTxt);
        }
    }
}

void CSIFrameWnd::OnSiWindowActivated(HWND hActived, HWND hDeactived)
{
    ASSERT(m_pMdiChildMng != NULL);
    m_TabbarWnd.OnSiWindowActivated(hActived, hDeactived);
}

void CSIFrameWnd::OnMdiWindowActivated(HWND hActived)
{
    ASSERT(m_pMdiChildMng != NULL);
    m_TabbarWnd.OnMdiWindowActivated(hActived);
}

void CSIFrameWnd::OnSiWindowCreated(HWND hNewWnd)
{
    ASSERT(m_pMdiChildMng != NULL);
    if(m_pMdiChildMng->AddMdiChildWnd(hNewWnd))
    {
        m_TabbarWnd.OnWindowOpened(hNewWnd);
    }
}

void CSIFrameWnd::OnSiWindowClosing(HWND hWnd)
{
    ASSERT(m_pMdiChildMng != NULL);
    m_TabbarWnd.OnWindowClosing(hWnd);
}
