#include "StdAfx.h"
#include "Shlwapi.h"
#include "SourceInsight.h"
#include "Options.h"
#include "LoadDll.h"

LPCTSTR splashWndText = _T("TabSi Splash");
LPCTSTR splashWndClassName = _T("TabSiSplashClass");

LPCTSTR sourceInsightTitle = _T("Source Insight");
LPCTSTR lpszSiFrameWndClass = _T("si_Frame");
LPCTSTR lpszSi4FrameWndClass = _T("si4_Frame");
LPCTSTR lpszChindFrameWndClass = _T("si_Sw");
LPCTSTR lpszMdiClientWndClass = _T("MDIClient");

LPCTSTR lpszDllFileName = _T("TabSiPlus.dll");
LPCTSTR splashBmpFileName = _T("TabSiHk.dll");
LPCTSTR lpszTextMark = _T(" with TabSiPlus");

BOOL IsSourceInsightFrameWnd(HWND hWnd)
{
    TCHAR szClassName[128],szTitle[256];
    
    int nRtn = ::GetClassName(hWnd,szClassName,128);
    if(nRtn == 0)
        return FALSE;

    nRtn = ::GetWindowText(hWnd,szTitle,256);
    if(nRtn == 0)
        return FALSE;

    //windows class name is "si_Frame" (si4_Frame for 4.x)and title has "Source  Insight", 
    //It can be determined that this is a source insight window
    if( ((lstrcmp(lpszSiFrameWndClass,szClassName) == 0) || (lstrcmp(lpszSi4FrameWndClass,szClassName) == 0))
        && (StrStr(szTitle, sourceInsightTitle) != NULL))
    {
        if(StrStr(szTitle,lpszTextMark) != NULL)//There is a hook mark, do not harass this window again
            return FALSE;

        return TRUE;
    }

    return FALSE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
    BOOL bSuccess = TRUE;
    if(hwnd != NULL && IsSourceInsightFrameWnd(hwnd))
    {
        if(lParam)
        {
            HWND *pHwnd = (HWND *)lParam;
            *pHwnd = hwnd;
            bSuccess = FALSE;//Find a Source Insight window, exit
        }
    }

    return bSuccess;
}

HWND FindSourceInsightFrameWindow()
{
    HWND hSiFrmWnd = NULL;
    
    BOOL bRtn = ::EnumWindows(EnumWindowsProc,(LPARAM)&hSiFrmWnd);
    if(!bRtn && hSiFrmWnd != NULL)
        return hSiFrmWnd;
    else
        return NULL;
}

BOOL HookSourceInsightWindow(HWND hSiWnd, LPCTSTR lpszHookDll)
{
    BOOL bSuccess = TRUE;
    DWORD processId;
    ::GetWindowThreadProcessId(hSiWnd,&processId);

#ifdef _DEBUG
    TCHAR s[200];
    wsprintf(s, _T("TabSiHost > LoadDllForRemoteThread for PID = %d\n"), processId);
    ::OutputDebugString( s );
#endif
    DWORD dwRet = LoadDllForRemoteThread(processId,TRUE,FALSE,lpszHookDll,"Initialize",NULL,NULL,NULL,NULL,NULL,0,NULL);
#ifdef _DEBUG
        wsprintf( s, _T("TabSiHost > LoadDllForRemoteThread for PID = %d returned %d (0x%X)\n"),    processId, dwRet, dwRet );
        ::OutputDebugString( s );
#endif
    if(dwRet != 0)
    {
        bSuccess = FALSE;
    }
    else
    {
        bSuccess = TRUE;
    }

    return bSuccess;
}

//Find and hook one Source Insight window
BOOL FindAndHookSourceInsightWindow(LPCTSTR lpszHookDll)
{
    BOOL bSuccess = FALSE;
    if(lpszHookDll)
    {
        HWND hSiFrmWnd = FindSourceInsightFrameWindow();
        if(hSiFrmWnd != NULL)
        {
            bSuccess = HookSourceInsightWindow(hSiFrmWnd,lpszHookDll);
        }
    }
    return bSuccess;
}

BOOL GetFileNameFromSiCaption(LPCTSTR lpszSiCaption, LPTSTR lpszBuffer, int nBufSize)
{
    lpszBuffer[0] = 0;
    if(!lpszSiCaption)
        return FALSE;

    int nLength = lstrlen(lpszSiCaption);
    if(nLength <= 2)
    {
        lstrcpy(lpszBuffer,lpszSiCaption);
        return FALSE;
    }

    BOOL bHasAsterisk = FALSE;
    LPCTSTR lpszRight = lpszSiCaption + (nLength - 2);
    if(lstrcmp(lpszRight,_T(" *")) == 0)
        bHasAsterisk = TRUE;

    LPTSTR lpHeader = (LPTSTR)lpszSiCaption;
    LPTSTR lpChar = _tcsstr(lpHeader,_T(" ("));
    if(lpChar != NULL)
    {
        int nCount = min(lpChar - lpHeader, nBufSize) + 1;
        lstrcpyn(lpszBuffer,lpHeader,nCount);
        lpszBuffer[nCount] = 0;
    }
    else
    {
        int nCount = min(_tcslen(lpHeader), nBufSize) + 1;
        lstrcpyn(lpszBuffer,lpHeader,nCount);
        lpszBuffer[nCount] = 0;
    }
    
    return bHasAsterisk;
}