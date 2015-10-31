// TabWndUIThread.cpp : implementation file
//

#include "stdafx.h"
#include "tabsiplus.h"
#include "TabWndUIThread.h"

#include "LoadDll.h"
#include "SourceInsight.h"

#include "SIFrameWnd.h"
#include "SiMDIWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTabWndUIThread *g_pTabWndUIThread = NULL;

/////////////////////////////////////////////////////////////////////////////
// CTabWndUIThread

IMPLEMENT_DYNCREATE(CTabWndUIThread, CWinThread)

CTabWndUIThread::CTabWndUIThread()
{
    m_bAutoDelete = FALSE;
}

CTabWndUIThread::~CTabWndUIThread()
{
}

BOOL CTabWndUIThread::InitInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SetThreadNativeLanguage();

    HWND hWndSIFrame = FindSourceInsightFrameWindow();
    if(hWndSIFrame == NULL)
        return FALSE;

    DWORD dwSIPID = 0;
    ::GetWindowThreadProcessId(hWndSIFrame,&dwSIPID);
    if(dwSIPID != ::GetCurrentProcessId())
        return FALSE;

    g_pSiFrameWnd = new CSIFrameWnd(hWndSIFrame);
    if(g_pSiFrameWnd->ShowTabbarWnd())
    {
        m_pMainWnd = g_pSiFrameWnd->GetTabbarWnd();
    }

//    g_pSiFrameWnd->SendMessage(WM_NCPAINT, (WPARAM)1, (LPARAM)0 );
    return TRUE;
}

int CTabWndUIThread::ExitInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if(g_pSiFrameWnd)
    {
        //g_pSiFrameWnd->CloseTabbarWnd();
        delete g_pSiFrameWnd;
        g_pSiFrameWnd = NULL;
    }
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTabWndUIThread, CWinThread)
    //{{AFX_MSG_MAP(CTabWndUIThread)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabWndUIThread message handlers

int CTabWndUIThread::Run() 
{
    // TODO: Add your specialized code here and/or call the base class
    int nCode = CWinThread::Run();    
/*
    if(gGlobalShutdown)
    {
        HMODULE hThisModule = ::GetModuleHandle(_T("TabSiPlus.dll"));
        if(hThisModule)
        {
            ::FreeLibrary(hThisModule);
        }
    }
*/
    return nCode;
}

